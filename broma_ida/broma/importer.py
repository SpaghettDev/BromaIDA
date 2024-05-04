from io import TextIOWrapper

from idaapi import get_imagebase
from idc import (
    get_name as get_ea_name, get_func_flags, get_func_cmt,
    set_func_cmt,
    FUNC_LIB
)
from ida_funcs import get_func, add_func
from ida_kernwin import ASKBTN_BTN1, ASKBTN_CANCEL

from re import sub

from broma_ida.pybroma import Root

from broma_ida.broma.constants import BROMA_PLATFORMS
from broma_ida.broma.binding import Binding
from broma_ida.utils import (
    get_platform_printable, get_short_info,
    rename_func, popup, stop,
    are_args_primitive
)


class BromaImporter:
    """Broma importer of all time using PyBroma now!"""
    _target_platform: BROMA_PLATFORMS
    _file_path: str

    bindings: list[Binding] = []
    # { 0xaddr: [Binding, Binding, ...] }
    duplicates: dict[int, list[Binding]] = {}

    def __init__(self, platform: BROMA_PLATFORMS):
        """Initializes a BromaImporter instance

        Args:
            platform (BROMA_PLATFORMS): The target platform
        """
        self._reset()
        self._target_platform = platform

    def parse_file_stream(self, file: TextIOWrapper):
        """Parses a .bro file from a file stream

        Args:
            file (TextIOWrapper): The file stream

        Returns:
            tuple[list[Binding], dict[int, list[Binding]]]:
            0 contains unique bindings, 1 contains duplicates
        """
        self._file_path = file.name
        root = Root(self._file_path)

        for class_name, broma_class in root.classesAsDict().items():
            for field in broma_class.fields:
                function_field = field.getAsFunctionBindField()

                if function_field is None:
                    continue

                function_address = function_field.binds.__getattribute__(
                    self._target_platform
                )

                if function_address == -1:
                    continue

                function = function_field.prototype

                # Runs only for the first time an address has a duplicate
                if function_address in self.bindings:
                    dup_binding = self.bindings[
                        self.bindings.index(function_address)
                    ]
                    error_location = \
                        f"{class_name}::{function.name} " \
                        f"and {dup_binding['qualifiedName']} " \
                        f"@ {hex(function_address)}"

                    if f"{class_name}::{function.name}" == \
                            dup_binding['qualifiedName']:
                        print(
                            "[!] BromaImporter: Duplicate binding with "
                            f"same qualified name! ({error_location})"
                        )
                        continue
                    elif class_name == dup_binding['className']:
                        print(
                            "[!] BromaImporter: Duplicate binding within "
                            f"same class! ({error_location})"
                        )
                        continue

                    print(
                        "[!] BromaImporter: Duplicate binding! "
                        f"({class_name}::{function.name} "
                        f"and {dup_binding['qualifiedName']} "
                        f"@ {hex(function_address)})"
                    )
                    del self.bindings[self.bindings.index(dup_binding)]
                    self.duplicates[function_address] = []
                    self.duplicates[function_address].append(dup_binding)

                if function_address in self.duplicates:
                    self.duplicates[function_address].append(Binding({
                        "name": function.name,
                        "className": class_name,
                        "address": function_address,
                        "return_type": function.ret,
                        "parameters": function.args
                    }))
                    continue

                self.bindings.append(Binding({
                    "name": function.name,
                    "className": class_name,
                    "address": function_address,
                    "return_type": function.ret,
                    "parameters": function.args
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

            # TODO: finish this
            # if are_args_primitive(binding["parameters"]):  # type: ignore
            #     SetType(get_screen_ea(), "")

            if ida_name.startswith("sub_"):
                rename_func(
                    ida_ea,
                    binding["idaQualifiedName"]  # type: ignore
                )
            elif sub("_[0-9]+", "", ida_name) != binding["idaQualifiedName"]:
                mismatch_popup = popup(
                    "Overwrite", "Keep", "",
                    f"""Mismatch in Broma ({binding["qualifiedName"]}) """
                    f"and idb ({ida_name})!\n"
                    "Overwrite from Broma or keep current name?"
                )

                if mismatch_popup == ASKBTN_BTN1:
                    rename_func(
                        ida_ea,
                        binding["idaQualifiedName"]  # type: ignore
                    )
                elif mismatch_popup == ASKBTN_CANCEL:
                    stop()

        # and now for what took me 3 hours :D
        for addr, bindings in self.duplicates.items():
            ida_ea = get_imagebase() + addr

            func_cmt = get_func_cmt(ida_ea, True)
            func_names = ", ".join(
                [binding["qualifiedName"]
                    for binding in bindings]  # type: ignore
            )

            if func_cmt == "":
                # use the first occurrence as the name (very good imo)
                rename_func(
                    ida_ea,
                    bindings[0]["idaQualifiedName"]  # type: ignore
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
                    "(You will be prompted with this again if you rerun the "
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
