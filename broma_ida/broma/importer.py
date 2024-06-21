from io import TextIOWrapper
from typing import cast, Optional

from idaapi import get_imagebase
from idc import (
    get_name as get_ea_name, get_func_flags, get_func_cmt,
    set_func_cmt, SetType,
    FUNC_LIB
)
from ida_funcs import get_func, add_func
from ida_kernwin import ASKBTN_BTN1, ASKBTN_CANCEL
from ida_typeinf import (
    get_idati, get_ordinal_qty,
    func_type_data_t as ida_func_type_data_t,
    tinfo_t as ida_tinfo_t
)
from ida_nalt import get_tinfo

from re import sub
from pathlib import Path

from pybroma import Root, FunctionBindField, Type, Class

from broma_ida.broma.constants import BROMA_PLATFORMS
from broma_ida.broma.binding import Binding
from broma_ida.broma.argtype import ArgType, RetType
from broma_ida.broma.codegen import BromaCodegen
from broma_ida.utils import (
    get_platform_printable, get_short_info,
    rename_func, popup, stop,
    get_ida_plugin_path
)


g_has_idaclang = False

try:
    from ida_srclang import (
        set_parser_argv, parse_decls_for_srclang,
        SRCLANG_CPP
    )
    g_has_idaclang = True
except ImportError:
    pass


class BIUtils:
    """BromaImporter utilities"""
    @staticmethod
    def get_function_info(ida_ea: int) -> Optional[ida_func_type_data_t]:
        """Gets the info about a function

        Args:
            ida_ea (int): The function address

        Returns:
            Optional[ida_func_type_data_t]: The ida_func_type_data_t
            of the function or None if not found
        """
        tif = ida_tinfo_t()
        func_info = ida_func_type_data_t()

        if get_tinfo(tif, ida_ea):
            if tif.get_func_details(func_info):
                return func_info

        return None

    @staticmethod
    def get_type_info(
        name: str,
        update: bool = False
    ) -> Optional[ida_tinfo_t]:
        """Gets the info about a type/struct

        Args:
            name (str): The name of the type/struct
            update (bool): Should update the cache. Defaults to False.

        Returns:
            Optional[ida_tinfo_t]: The ida_tinfo_t of the type
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
    def from_pybroma_args(args: dict[str, Type]) -> list[ArgType]:
        """Converts pybroma args into a list of ArgTypes

        Args:
            args (dict[str, Type]): The pybroma args

        Returns:
            list[ArgType]
        """
        return [
            ArgType({
                "name": name,
                "type": arg_t.name.replace("gd::", "std::")
            })
            for name, arg_t in args.items()
        ]

    @staticmethod
    def verify_type(t: Optional[ida_tinfo_t]) -> bool:
        if t is None:
            return True

        if t.get_size() == 0xFFFFFFFFFFFFFFFF:
            return True

        return False


class BromaImporter:
    """Broma importer of all time using PyBroma now!"""
    _target_platform: BROMA_PLATFORMS
    _file_path: str
    _has_types: bool = False

    bindings: list[Binding] = []
    # { 0xaddr: [Binding, Binding, ...] }
    duplicates: dict[int, list[Binding]] = {}

    # Signature specific functions below
    def _has_mismatch(
        self,
        function: ida_func_type_data_t,
        binding: Binding
    ) -> bool:
        """Checks if there is a mismatch between the idb and a binding.

        Args:
            function (func_type_data_t):
                The function signature returned by IDA.
            binding (Binding): The binding.

        Returns:
            bool
        """
        # TODO: impl
        return True

    def _get_ida_args_str(
        self,
        binding: Binding
    ) -> str:
        """Gets a function's argument string.

        Args:
            binding (Binding): The binding

        Returns:
            str
        """
        this_arg_or_empty: str = ""

        if not binding["is_static"]:
            this_arg_or_empty += f"""{binding["class_name"]}* this"""
            if len(binding["parameters"]) > 0:
                this_arg_or_empty += ", "

        return this_arg_or_empty + ", ".join([
            str(arg) for arg in binding["parameters"]
        ])

    def _get_function_signature(
        self,
        binding: Binding
    ) -> str:
        """Returns a C++ function signature for the given function.

        Args:
            binding (Binding): The binding.

        Returns:
            str: The C++ function signature
        """
        return \
            f"""{"static " if binding["is_static"] else ""}""" \
            f"""{"virtual " if binding["is_virtual"] else ""}""" \
            f"""{binding["return_type"]["type"]} """ \
            f"""{binding["ida_qualified_name"]}""" \
            f"""({self._get_ida_args_str(binding)});"""
    # End of signature specific functions

    def _codegen_classes(self, classes: dict[str, Class]) -> bool:
        """Codegens the file that contains the parsed broma classes

        Args:
            classes (dict[str, Class])

        Returns:
            bool
        """
        if not g_has_idaclang:
            return False

        plugin_path = get_ida_plugin_path()

        if plugin_path is None:
            popup(
                "Ok", "Ok", None,
                "TODO"
            )
            return False

        BromaCodegen(
            self._target_platform,
            classes,
            plugin_path / "broma_ida" / "types",
            Path("/".join(Path(self._file_path).parts[:-1]))
        ).write()

        return True

    def __init__(self, platform: BROMA_PLATFORMS):
        """Initializes a BromaImporter instance

        Args:
            platform (BROMA_PLATFORMS): The target platform
        """
        self._reset()
        self._target_platform = platform

    def parse_file_stream(self, file: TextIOWrapper):
        """Parses a .bro file from a file stream
        and imports the members and methods

        Args:
            file (TextIOWrapper): The file stream

        Returns:
            tuple[list[Binding], dict[int, list[Binding]]]:
            0 contains unique bindings, 1 contains duplicates
        """
        self._file_path = file.name
        root = Root(self._file_path)

        if g_has_idaclang and popup(
            "Yes", "No", None, "Import classes from Broma?"
        ) == ASKBTN_BTN1:
            if self._codegen_classes(root.classesAsDict()):
                if self._target_platform == "win":
                    set_parser_argv("clang", "-x c++ -target x86_64-pc-win32")
                # TODO: handle other platforms

                popup(
                    "Ok", "Ok", None,
                    "Importing Types...\n"
                    "This will probably freeze IDA for a couple of "
                    "seconds, if not minutes.\n"
                    "So don't close the plugin/IDA!\n"
                    "(This will only happen once, unless new "
                    "types are found in Broma)"
                )

                parse_decls_for_srclang(
                    SRCLANG_CPP,
                    None,
                    (
                        get_ida_plugin_path() / "broma_ida" / "types" / "codegen.hpp"  # type: ignore
                    ).as_posix(),
                    True
                )

                self._has_types = True
        else:
            holy_shit_struct = BIUtils.get_type_info("holy_shit")

            if holy_shit_struct:
                self._has_types = holy_shit_struct.get_size() == 0x808

            if any([
                BIUtils.verify_type(BIUtils.get_type_info(t))
                for t in (
                    "cocos2d::CCObject", "cocos2d::CCImage",
                    "cocos2d::CCApplication", "cocos2d::CCDirector"
                )
            ]):
                self._has_types = False

                popup(
                    "Ok", "Ok", None,
                    "Mismatch in cocos2d types! "
                    "Function types will not be changed!"
                )

        for class_name, broma_class in root.classesAsDict().items():
            for field in broma_class.fields:
                function_field = field.getAsFunctionBindField()

                if function_field is None:
                    continue

                function_address = function_field.binds.__getattribute__(
                    self._target_platform
                )

                if function_address == -1 or function_address == -2:
                    continue

                function = function_field.prototype

                # Runs only for the first time an address has a duplicate
                if function_address in self.bindings:
                    dup_binding = self.bindings[
                        self.bindings.index(function_address)
                    ]
                    error_location = \
                        f"{class_name}::{function.name} " \
                        f"and {dup_binding['qualified_name']} " \
                        f"@ {hex(function_address)}"

                    if f"{class_name}::{function.name}" == \
                            dup_binding['qualified_name']:
                        print(
                            "[!] BromaImporter: Duplicate binding with "
                            f"same qualified name! ({error_location})"
                        )
                        continue
                    elif class_name == dup_binding['class_name']:
                        print(
                            "[!] BromaImporter: Duplicate binding within "
                            f"same class! ({error_location})"
                        )
                        continue

                    print(
                        "[!] BromaImporter: Duplicate binding! "
                        f"({class_name}::{function.name} "
                        f"and {dup_binding['qualified_name']} "
                        f"@ {hex(function_address)})"
                    )
                    del self.bindings[self.bindings.index(dup_binding)]
                    self.duplicates[function_address] = []
                    self.duplicates[function_address].append(dup_binding)

                if function_address in self.duplicates:
                    self.duplicates[function_address].append(Binding({
                        "name": function.name,
                        "class_name": class_name,
                        "address": function_address,
                        "return_type": RetType({
                            "name": "",
                            "type": function.ret.name
                        }),
                        "parameters": BIUtils.from_pybroma_args(function.args),
                        "is_virtual": function.is_virtual,
                        "is_static": function.is_static
                    }))
                    continue

                self.bindings.append(Binding({
                    "name": function.name,
                    "class_name": class_name,
                    "address": function_address,
                    "return_type": RetType({
                        "name": "",
                        "type": function.ret.name
                    }),
                    "parameters": BIUtils.from_pybroma_args(function.args),
                    "is_virtual": function.is_virtual,
                    "is_static": function.is_static
                }))

        file.close()

        print(
            f"\n\n[+] BromaImporter: Read {len(self.bindings)} "
            f"{get_platform_printable(self._target_platform)} bindings "
            f"and {len(self.duplicates)} duplicates "
            f"from {self._file_path}\n\n"
        )

    def import_into_idb(self):
        """Imports the bindings into the current idb"""

        # first, handle non-duplicates
        for binding in self.bindings:
            ida_ea = get_imagebase() + binding["address"]
            ida_name = get_ea_name(ida_ea)
            ida_func_flags = get_func_flags(ida_ea)

            if ida_name.startswith("loc_"):
                add_func(ida_ea)

            if ida_func_flags & FUNC_LIB:
                print(
                    f"[!] BromaImporter: Tried to rename a library function! "
                    f"({get_short_info(binding)})"
                )
                continue

            if get_func(ida_ea).start_ea != ida_ea:
                print(
                    f"[!] BromaImporter: Function is in the middle of "
                    f"another one! ({get_short_info(binding)})"
                )
                continue

            if self._has_types and \
                self._has_mismatch(cast(
                    ida_func_type_data_t,
                    BIUtils.get_function_info(ida_ea)
                    ), binding
                ):
                SetType(ida_ea, self._get_function_signature(binding))

            if ida_name.startswith("sub_"):
                rename_func(
                    ida_ea,
                    binding["ida_qualified_name"]
                )
            elif sub("_[0-9]+", "", ida_name) != binding["ida_qualified_name"]:
                mismatch_popup = popup(
                    "Overwrite", "Keep", "",
                    f"""Mismatch in Broma ({binding["qualified_name"]}) """
                    f"and idb ({ida_name})!\n"
                    "Overwrite from Broma or keep current name?"
                )

                if mismatch_popup == ASKBTN_BTN1:
                    rename_func(
                        ida_ea,
                        binding["ida_qualified_name"]
                    )
                elif mismatch_popup == ASKBTN_CANCEL:
                    stop()

        # and now for what took me countless hours of debugging :D
        for addr, bindings in self.duplicates.items():
            ida_ea = get_imagebase() + addr

            func_cmt = get_func_cmt(ida_ea, True)
            func_names = ", ".join(
                [binding["qualified_name"] for binding in bindings]
            )

            if func_cmt == "":
                # use the first occurrence as the name (very good imo)
                rename_func(
                    ida_ea,
                    bindings[0]["ida_qualified_name"]
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
                if popup(
                    "Overwrite", "Keep", None,
                    f"{hex(addr)} already has a comment! Would you like to "
                    "overwrite it with merge information or keep the current "
                    "comment?\n"
                    "(You will be prompted with this again if you keep the "
                    "current comment and rerun the "
                    "script and there are merged functions!)"
                ) == ASKBTN_BTN1:
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
