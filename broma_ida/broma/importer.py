from copy import deepcopy
from collections import deque

from idaapi import (
    get_imagebase, apply_tinfo,
    GN_SHORT, GN_DEMANGLED, TINFO_DEFINITE, BTF_TYPEDEF,
    BADADDR
)
from idc import (
    get_name as get_ea_name, get_func_flags, get_func_cmt,
    set_func_cmt, SetType,
    FUNC_LIB
)
from ida_funcs import get_func, add_func
from ida_kernwin import (
    show_wait_box, hide_wait_box, warning as ida_warning,
    ASKBTN_BTN1
)
try:
    from ida_typeinf import get_ordinal_qty
except ImportError:
    from ida_typeinf import get_ordinal_count  # type: ignore
    get_ordinal_qty = get_ordinal_count
from ida_typeinf import (
    get_idati, set_c_header_path,
    func_type_data_t as ida_func_type_data_t,
    tinfo_t as ida_tinfo_t
)
from idautils import Names

from re import sub
from pathlib import Path
from hashlib import file_digest

from pybroma import Root, Class

from broma_ida.broma.constants import BROMA_PLATFORMS
from broma_ida.broma.binding import Binding
from broma_ida.broma.codegen import BromaCodegen
from broma_ida.utils import path_exists, stop, IDAUtils, HAS_IDACLANG

from broma_ida.data.data_manager import DataManager

from broma_ida.ui.simple_popup import SimplePopup
from broma_ida.ui.directory_input_form import DirectoryInputForm
from broma_ida.ui.ask_popup import AskPopup

if HAS_IDACLANG:
    from ida_srclang import (
        set_parser_argv, parse_decls_for_srclang,
        SRCLANG_CPP
    )


class BIUtils:
    """BromaImporter utilities"""

    _common_clang_argv = "-x c++ -nostdlib -nostdinc -nostdinc++"

    _plat_to_parser_argv: dict[BROMA_PLATFORMS, str] = {
        "win": "-target x86_64-pc-win32",
        "imac": "-target x86_64-apple-darwin",
        "m1":  "-target arm64-apple-darwin",
        "ios": "-target arm64-apple-darwin",
        "android32":  "-target armv7-none-linux-androideabi -mfloat-abi=hard",
        "android64": "-target aarch64-none-linux-android -mfloat-abi=hard"
    }

    _plat_to_hss_size: dict[BROMA_PLATFORMS, int] = {
        "win": 0x8D0,
        "imac": 0x738,
        "m1": 0x738,
        "ios": 0x738,
        "android32": 0x4EC,
        "android64": 0x9D8
    }

    _plat_to_stl_name: dict[BROMA_PLATFORMS, str] = {
        "win": "windows",
        "android32": "android",
        "android64": "android",
        "imac": "macho",
        "m1": "macho",
        "ios": "macho"
    }

    @staticmethod
    def get_type_info(
        name: str,
        update: bool = False
    ) -> ida_tinfo_t | None:
        """Gets the info about a type/struct

        Args:
            name (str): The name of the type/struct
            update (bool): Should update the cache. Defaults to False.

        Returns:
            ida_tinfo_t | None: The ida_tinfo_t of the type
            or None if not found
        """
        if not hasattr(BIUtils.get_type_info, "types") or update:
            BIUtils.get_type_info.types = {}

            idati = get_idati()
            tif = ida_tinfo_t()

            for ordinal in range(1, get_ordinal_qty(idati) + 1):
                if tif.get_numbered_type(idati, ordinal):
                    BIUtils.get_type_info.types[tif.get_type_name()] = \
                        tif.copy()

        if name in BIUtils.get_type_info.types:
            return BIUtils.get_type_info.types[name]

        return None

    @staticmethod
    def verify_type(t: ida_tinfo_t | None) -> bool:
        """Verifies an ida tinfo_t

        Args:
            t (ida_tinfo_t | None):

        Returns:
            bool: True on error
        """
        if t is None:
            return False

        if t.get_size() == BADADDR or t.is_forward_decl():
            return True

        return False

    @staticmethod
    def verify_types(platform: BROMA_PLATFORMS) -> bool:
        """Verifies the existance and size of types

        Returns:
            bool: True on success
        """
        if DataManager().get("ignore_mismatched_structs"):
            return True

        holy_shit_struct = BIUtils.get_type_info("holy_shit")

        if holy_shit_struct:
            if holy_shit_struct.get_size() != \
                    BIUtils.get_holy_shit_struct_size(platform):
                ida_warning(
                    "Mismatch in STL types! "
                    "Classes will not be imported!\n"
                    "To fix this, go to the local types window, "
                    "delete all Cocos and GD types\n"
                    "(as well as holy_shit struct) then save the IDB."
                )
                return False

        if any((
            BIUtils.verify_type(BIUtils.get_type_info(t))
            for t in (
                "cocos2d::CCObject", "cocos2d::CCNode", "cocos2d::CCImage",
                "cocos2d::CCApplication", "cocos2d::CCDirector"
            )
        )):
            ida_warning(
                "Mismatch in cocos2d types! "
                "Classes will not be imported!\n"
                "To fix this, go to the local types window, "
                "and delete all Cocos and GD types\n"
                "(as well as holy_shit struct) then save the IDB."
            )
            return False

        return True

    @staticmethod
    def get_holy_shit_struct_size(platform: BROMA_PLATFORMS) -> int:
        """Gets the size of the STL struct for the supplied platform

        Args:
            platform (BROMA_PLATFORMS)

        Returns:
            int: size in bytes
        """
        return BIUtils._plat_to_hss_size[platform]

    @staticmethod
    def get_parser_argv(platform: BROMA_PLATFORMS) -> str:
        """Gets the parser arguments for a certain platform

        Args:
            platform (BROMA_PLATFORMS)

        Returns:
            str
        """
        return f"""{
            BIUtils._common_clang_argv
        } {BIUtils._plat_to_parser_argv[platform]}"""

    @staticmethod
    def get_stl_headers_path(platform: BROMA_PLATFORMS) -> str:
        """Gets the STL headers path for a given platform

        Args:
            platform (BROMA_PLATFORMS)

        Returns:
            str
        """
        return IDAUtils.get_ida_path(
            f"""plugins/broma_ida/types/c++stl/{
                BIUtils._plat_to_stl_name[platform]
            }"""
        ).as_posix()

    @staticmethod
    def prompt_invalid_dir(input_str: str, dm_key: str):
        """Shows a warning and prompts the user to input a valid directory.
        Saves the directory to the DataManager

        Args:
            input_str (str)
            dm_key (str)
        """
        ida_warning(
            f"Importing types with an invalid {input_str}!\n"
            "Please set one!"
        )
        dir_form = DirectoryInputForm(input_str)
        dir_form.show()

        dir_str = dir_form.saved_controls.iDir

        if not path_exists(dir_str):
            ida_warning("bruh")
            stop()

        DataManager().set(dm_key, dir_str)

    # Signature stuff

    @staticmethod
    def has_mismatch(function: ida_func_type_data_t, binding: Binding) -> bool:
        """Checks if there is a mismatch between the idb and a binding.

        Args:
            function (func_type_data_t):
                The function signature returned by IDA.
            binding (Binding): The binding.

        Returns:
            bool
        """
        if function is None:
            return True

        if function.rettype != binding.ret:
            return True

        for i, arg in enumerate(function):
            if i == 0 and not binding.is_static:
                if str(arg.type) != f"""{binding.class_name} *""":
                    return True
            elif str(arg).replace(" *", "*") != binding.parameters[
                i - (0 if binding.is_static else 1)
            ].type:
                return True

        return False

    @staticmethod
    def set_function_signature(ea: int, binding: Binding):
        """Set's the function at `ea`'s signature. Has custom logic for
        functions that use STL types since those break when using SetType
        because of the commas in the template arguments

        Args:
            ea (int):
            binding (Binding):
        """
        args_has_stl_type = False
        ret_has_stl_type = False

        # we don't have an issue with std::string, only with generic stl types
        if "std::" in binding.ret.type and \
                binding.ret.stripped_type != "std::string":
            ret_has_stl_type = True

        for parameter in binding.parameters:
            if parameter.stripped_type == "std::string":
                continue

            if "std::" in parameter.type:
                args_has_stl_type = True
                break

        if not args_has_stl_type and not ret_has_stl_type:
            SetType(ea, binding.signature)
            return

        binding_fix = deepcopy(binding)
        arg_stl_idx: list[int] = []

        if args_has_stl_type:
            for i in range(len(binding_fix.parameters)):
                if "std::" in binding_fix.parameters[i].type:
                    arg_stl_idx.append(i)
                    binding_fix.parameters[i].type = "void*"

        if ret_has_stl_type:
            binding_fix.ret.type = "void*"

        # first set correct amount of arguments
        SetType(ea, binding_fix.signature)

        function_data = IDAUtils.get_function_info(ea, True)

        if function_data is None:
            ida_warning(
                "Couldn't fix STL parameters for "
                f"function {binding.qualified_name}!\n"
                "(function is null)"
            )
            return

        # then fix the arguments
        for idx in arg_stl_idx:
            stl_type = ida_tinfo_t()
            stl_type.get_named_type(
                get_idati(),
                binding.parameters[idx].stripped_type,
                BTF_TYPEDEF,
                False
            )

            if stl_type.get_ordinal() == 0:
                ida_warning(
                    f"STL Type '{stl_type.get_type_name()}' "
                    "isn't present in the type library!\n"
                    "Please open a GitHub issue."
                )
                return

            if binding.parameters[idx].type.endswith("&") or \
                    binding.parameters[idx].type.endswith("*"):
                stl_type_ptr = ida_tinfo_t()
                stl_type_ptr.create_ptr(stl_type)

                stl_type = stl_type_ptr

            if "const" in binding.parameters[idx].type:
                stl_type.set_const()

            try:
                function_data[
                    idx + (0 if binding.is_static else 1)
                ].type = stl_type
            except IndexError:
                ida_warning(
                    "Couldn't fix STL parameters for "
                    f"function {binding.qualified_name}!\n"
                    "(parameter index out of range)"
                )
                return

        if ret_has_stl_type:
            stl_type = ida_tinfo_t()
            stl_type.get_named_type(
                get_idati(),
                binding.ret.stripped_type,
                BTF_TYPEDEF,
                False
            )

            if binding.ret.type.endswith("&") or \
                    binding.ret.type.endswith("*"):
                stl_type_ptr = ida_tinfo_t()
                stl_type_ptr.create_ptr(stl_type)

                stl_type = stl_type_ptr

            function_data.rettype = stl_type

        func_tinfo = ida_tinfo_t()
        func_tinfo.create_func(function_data)

        # and finally apply the actual correct type
        apply_tinfo(ea, func_tinfo, TINFO_DEFINITE)


# TODO: split into BromaTypesImporter and BromaFunctionsImpoter
class BromaImporter:
    """Broma importer of all time using PyBroma now!"""

    _target_platform: BROMA_PLATFORMS
    _file_path: str
    _has_types: bool = False

    bindings: deque[Binding] = deque()
    duplicates: dict[int, list[Binding]] = {}

    def _codegen_classes(self, classes: dict[str, Class]) -> bool:
        """Codegens the file that contains the parsed broma classes

        Args:
            classes (dict[str, Class])

        Returns:
            bool
        """
        if not HAS_IDACLANG:
            return False

        BromaCodegen(
            self._target_platform,
            classes,
            IDAUtils.get_ida_path("plugins") / "broma_ida" / "types",
            Path("/".join(Path(self._file_path).parts[:-1]))
        ).write()

        return True

    def __init__(self, platform: BROMA_PLATFORMS, filepath: str):
        """Initializes a BromaImporter instance

        Args:
            platform (BROMA_PLATFORMS): The target platform
            filepath (str): The Broma file's path
        """
        self._reset()
        self._target_platform = platform
        self._file_path = filepath

    def parse_file(self):
        """
        Parses the broma file passed into the constructor
        and imports the members and methods
        """
        root = Root(self._file_path)
        import_types: bool = DataManager().get("import_types")

        if not HAS_IDACLANG and import_types:
            ida_warning(
                "Trying to import types without IDAClang! "
                "Disabling importing of types..."
            )
            DataManager().set("import_types", False)
            import_types = False

        if import_types and not DataManager().get("disable_broma_hash_check"):
            cur_hash: str
            with open(self._file_path, "rb", buffering=0) as f:
                cur_hash = file_digest(f, "sha256").hexdigest()  # type: ignore

            last_broma_info = DataManager().get(
                "last_broma_info", (self._target_platform, "")
            )

            # Python>local_t_tree = ida_dirtree.get_std_dirtree(
            #   ida_dirtree.DIRTREE_LOCAL_TYPES
            # )
            # Python>local_t_tree.mkdir("test")
            # Python>local_t_tree.chdir("test")
            # 0x2
            # Python>local_t_tree.getcwd()
            # '/test'
            # Python>local_t_tree.link("_GUID")
            # 0x0

            if last_broma_info[0] == self._target_platform and \
                    last_broma_info[1] != "":
                if last_broma_info[1] == cur_hash:
                    print(
                        "[!] BromaImporter: Detected same Broma hash. "
                        "Will not import types..."
                    )
                    import_types = False
            elif last_broma_info[1] == "":
                DataManager().set(
                    "last_broma_info", (self._target_platform, cur_hash)
                )

        if import_types:
            set_c_header_path(
                BIUtils.get_stl_headers_path(self._target_platform)
            )

        if HAS_IDACLANG and import_types:
            if BIUtils.verify_types(self._target_platform) and \
                    self._codegen_classes(root.classesAsDict()):
                if DataManager().get("set_default_parser_args"):
                    set_parser_argv(
                        "clang",
                        BIUtils.get_parser_argv(self._target_platform)
                    )

                SimplePopup(
                    "Importing Types...\n"
                    "This will probably freeze IDA for a couple of "
                    "seconds, if not minutes.\n"
                    "Click on 'OK' to confirm.",
                    "OK"
                ).show()

                show_wait_box("HIDECANCEL\nImporting types...")

                parse_decls_for_srclang(
                    SRCLANG_CPP,
                    None,
                    IDAUtils.get_ida_path(
                        "plugins/broma_ida/types/codegen/"
                        f"{self._target_platform}.hpp"
                    ).as_posix(),
                    True
                )

                hide_wait_box()

                self._has_types = True
            else:
                # TODO: check if idb already has types, if so set this to True
                # so you dont HAVE to import types to be able to modify
                # function signatures
                self._has_types = False

        if self._target_platform.startswith("android"):
            for class_name, broma_class in root.classesAsDict().items():
                for field in broma_class.fields:
                    function_field = field.getAsFunctionBindField()

                    if function_field is None:
                        continue

                    self.bindings.append(
                        Binding.from_pybroma(class_name, function_field)
                    )
        else:
            for class_name, broma_class in root.classesAsDict().items():
                for field in broma_class.fields:
                    function_field = field.getAsFunctionBindField()

                    if function_field is None:
                        continue

                    func_addr = int(
                        function_field.binds.platforms_as_dict().get(
                            self._target_platform, "-0x1"
                        ), 16
                    )

                    if func_addr == -1:
                        continue

                    function = function_field.prototype

                    # Runs only for the first time an address has a duplicate
                    if func_addr in self.bindings:
                        dup_binding = self.bindings[
                            self.bindings.index(func_addr)  # type: ignore
                        ]
                        error_location = \
                            f"{class_name}::{function.name} " \
                            f"and {dup_binding.short_info}"

                        if f"{class_name}::{function.name}" == \
                                dup_binding.qualified_name:
                            print(
                                "[!] BromaImporter: Duplicate binding with "
                                f"same qualified name! ({error_location})"
                            )
                            continue
                        elif class_name == dup_binding.class_name:
                            print(
                                "[!] BromaImporter: Duplicate binding within "
                                f"same class! ({error_location})"
                            )
                            continue

                        print(
                            "[!] BromaImporter: Duplicate binding! "
                            f"({class_name}::{function.name} "
                            f"and {dup_binding.short_info}"
                        )
                        self.bindings.remove(dup_binding)
                        self.duplicates[func_addr] = []
                        self.duplicates[func_addr].append(dup_binding)

                    if func_addr in self.duplicates:
                        self.duplicates[func_addr].append(
                            Binding.from_pybroma(class_name, function_field)
                        )
                        continue

                    self.bindings.append(
                        Binding.from_pybroma(class_name, function_field)
                    )

        print(
            f"\n\n[+] BromaImporter: Read {len(self.bindings)} "
            f"{IDAUtils.get_platform_printable()} bindings "
            f"and {len(self.duplicates)} duplicates "
            f"from {self._file_path}\n\n"
        )

    def import_into_idb(self):
        """Imports the bindings into the current idb"""
        if self._target_platform.startswith("android"):
            if not self._has_types:
                return

            ida_addresses: dict[str, int] = {}

            for addr, _ in Names():
                demangled_name = sub(
                    r"(\S+)::(\S+)\(.*\)",
                    r"\1::\2",
                    get_ea_name(addr, GN_SHORT | GN_DEMANGLED)
                )
                ida_addresses[demangled_name] = addr

            while self.bindings:
                binding = self.bindings.pop()

                ida_ea = ida_addresses.get(binding.qualified_name, -0x1)

                if ida_ea == -0x1:
                    continue

                if BIUtils.has_mismatch(
                        IDAUtils.get_function_info(ida_ea),
                        binding
                ):
                    BIUtils.set_function_signature(ida_ea, binding)

            return

        # first, handle non-duplicates
        while self.bindings:
            binding = self.bindings.pop()

            ida_ea: int = get_imagebase() + binding.address
            ida_name: str = get_ea_name(ida_ea)
            ida_func_flags: int = get_func_flags(ida_ea)

            if ida_name.startswith("loc_"):
                add_func(ida_ea)

            if ida_func_flags & FUNC_LIB:
                print(
                    f"[!] BromaImporter: Tried to rename a library function! "
                    f"({binding.short_info})"
                )
                continue

            if get_func(ida_ea).start_ea != ida_ea:
                print(
                    f"[!] BromaImporter: Function is in the middle of "
                    f"another one! ({binding.short_info})"
                )
                continue

            if self._has_types and BIUtils.has_mismatch(
                    IDAUtils.get_function_info(ida_ea),
                    binding
            ):
                BIUtils.set_function_signature(ida_ea, binding)

            if ida_name.startswith("sub_"):
                IDAUtils.rename_func(
                    ida_ea,
                    binding.ida_qualified_name
                )
            elif sub("_[0-9]+", "", ida_name) != binding.ida_qualified_name:
                if DataManager().get("always_overwrite_idb") or \
                    AskPopup(
                        f"""Mismatch in Broma ({binding.qualified_name}) """
                        f"and idb ({ida_name})!\n"
                        "Overwrite from Broma or keep current name?",
                        "Overwrite", "Keep",
                        icon="WARNING"
                ).show() == ASKBTN_BTN1:
                    IDAUtils.rename_func(
                        ida_ea,
                        binding.ida_qualified_name
                    )

        # and now handle duplicates
        for addr, bindings in self.duplicates.items():
            ida_ea = get_imagebase() + addr

            func_cmt: str = get_func_cmt(ida_ea, True)
            func_names = ", ".join(
                [binding.qualified_name for binding in bindings]
            )

            if func_cmt == "":
                # use the first occurrence as the name (very good imo)
                IDAUtils.rename_func(
                    ida_ea,
                    bindings[0].ida_qualified_name
                )

                set_func_cmt(ida_ea, f"Merged with: {func_names}", True)
            elif func_cmt.startswith("Merged with: "):
                cmt_func_names = func_cmt.lstrip("Merged with: ")

                if set(func_names.split(", ")) != \
                        set(cmt_func_names.split(", ")):
                    print(
                        "[!] BromaImporter: Mismatch in merged function list "
                        f"(Current: {cmt_func_names} | "
                        f"Correct: {func_names})! Correcting..."
                    )
                    set_func_cmt(
                        ida_ea, f"Merged with: {func_names}", True
                    )
            else:
                if DataManager().get(
                        "always_overwrite_merge_information"
                    ) or AskPopup(
                        f"{hex(addr)} already has a comment! "
                        "Would you like to overwrite it with "
                        "merge information or keep the current comment?\n"
                        "(You will be prompted with this again if you "
                        "keep the current comment and rerun the "
                        "script and there are merged functions!)\n"
                        "(You can enable 'Always overwrite function "
                        "comments with merge information' in settings "
                        "to get rid of this popup)",
                        "Overwrite", "Keep"
                ).show() == ASKBTN_BTN1:
                    set_func_cmt(
                        ida_ea, f"Merged with: {func_names}", True
                    )

    def _reset(self):
        """Resets a BromaParser instance because not doing so results
        in a re-run of the script populating the same bindings list"""
        self._target_platform = ""  # type: ignore
        self._file_path = ""
        self.bindings.clear()
        self.duplicates.clear()
