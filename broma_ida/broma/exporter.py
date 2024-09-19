from typing import cast, Generator, Any
from os import path

from idc import get_type
from idaapi import get_imagebase
from ida_kernwin import ASKBTN_BTN1

from re import search, match, sub, Match
from shutil import move

from broma_ida.broma.constants import (
    BROMA_PLATFORMS, CPP_TYPE_SPECIFIERS, CPP_TYPE_QUALIFIERS
)
from broma_ida.broma.binding import Binding
from broma_ida.broma.argtype import ArgType, RetType
from broma_ida.utils import IDAUtils, HAS_IDACLANG

from broma_ida.data.data_manager import DataManager

from broma_ida.ui.ask_popup import AskPopup


class BEUtils:
    """BromaExporter utilities"""

    @staticmethod
    def from_ida_args(ida_ea: int) -> list[ArgType]:
        """Creates a list of ArgTypes of a function from an offset.

        Args:
            ea (int):

        Returns:
            list[ArgType]
        """
        func_info = IDAUtils.get_function_info(ida_ea, True)

        # fallback to parsing function signature
        if func_info is None:
            sig: str = get_type(ida_ea)
            args = sig[sig.index("(")+1:-1]

            # just lie since get_type returns the "this"
            # argument already included
            return BEUtils.parse_str_args(args, "", True)

        args = [
            ArgType(str(arg.type), arg.name) for arg in func_info
        ]

        # remove return ptr argument
        if "std::" in str(func_info.rettype):
            args = args[1:]

        return args

    @staticmethod
    def parse_str_args(
            args_str: str | Any,
            class_name: str,
            is_static: bool
    ) -> list[ArgType]:
        """Mini parser that converts a string of arguments
        into a list of ArgTypes

        Args:
            args_str (str):
            class_name (str):
            is_static (bool):

        Returns:
            list[ArgType]
        """
        argstype_list: list[ArgType] = []

        if not is_static:
            argstype_list.append(ArgType(f"{class_name}*", "this"))

        current_token: str = ""
        nest_count = 0

        for i, c in enumerate(args_str):
            if c == "<":
                nest_count += 1

            if c == ">":
                nest_count -= 1

            if nest_count == 0 and \
                    (c == "," or i == len(args_str) - 1):
                if i == len(args_str) - 1:
                    current_token += c

                arg = current_token.lstrip()

                parts = arg.split()
                # parts with no specifiers nor qualifiers
                # this isn't actually used to build the ArgType
                parts_nosq = arg.split()

                for x in CPP_TYPE_SPECIFIERS + CPP_TYPE_QUALIFIERS:
                    if x in parts_nosq:
                        parts_nosq.remove(x)

                if "long" in parts_nosq:
                    try:
                        # replace "long long" with "long"
                        if parts_nosq[parts_nosq.index("long") + 1] == "long":
                            parts_nosq.pop(parts_nosq.index("long") + 1)
                        # replace "long int" with "long"
                        if parts_nosq[parts_nosq.index("long") + 1] == "int":
                            parts_nosq.pop(parts_nosq.index("long") + 1)
                    except ValueError:
                        pass

                if "short" in parts_nosq:
                    try:
                        # replace "short int" with "short"
                        if parts_nosq[parts_nosq.index("short") + 1] == "int":
                            parts_nosq.pop(parts_nosq.index("short") + 1)
                    except ValueError:
                        pass

                arg_type: str
                arg_name: str

                if len(parts_nosq) > 1 and \
                        not parts_nosq[-1].endswith("*") and \
                        not parts_nosq[-1].endswith("&"):
                    arg_type = " ".join(parts[:-1])
                    arg_name = parts[-1]
                else:
                    arg_type = " ".join(parts)
                    arg_name = f"p{len(argstype_list)}"

                argstype_list.append(
                    ArgType(arg_type, arg_name)
                )

                current_token = ""

                continue

            current_token += c

        return argstype_list


class BromaExporter:
    """
    Broma exporter of all time using regex
    (if you couldn't already tell)
    (i think i should invest in a parser)
    TODO: Use pybroma.BromaWriter when it becomes good
    """

    RX_CLASS = r"""class (\S+)"""
    """
    group 1: class name
    """

    RX_METHOD = \
        r"""^(?:\t| {4})(?:\/\/ )?(virtual|callback|static|)(?: *)?(?:((?:const )?\S+) )?(\S+)\((.*)\)(?: const|)(?: = ((?=inline).* \{|(?!inline)(?:(?:win|imac|m1|ios) (?:0[xX][0-9a-fA-F]+|inline)(?:, )?)+))?"""  # noqa: E501
    """
    group 1: "virtual", "callback", "static", or empty
    group 2: return type, empty if ctor/dtor
    group 3: function name
    group 4: comma-separated parameters or empty
    group 5: comma-separated platform address(es)
    """
    RX_METHOD_PLAT_ADDR_BASE = r"""{platform} (0[xX][0-9a-fA-F]+|inline)"""
    """
    group 1: binding address or "inline"
    """

    _filepath: str = ""
    _target_platform: BROMA_PLATFORMS

    bindings: dict[str, Binding] = {}
    """{ "qualified_name": Binding }"""
    overloads: dict[str, list[Binding]] = {}
    """{ "qualified_name": [Binding, Binding, ...] }"""
    num_exports: int = 0
    """Number of exported bindings"""
    num_ret_exports: int = 0
    """Number of exported return types"""
    num_args_names_exports: int = 0
    """Number of exported renamed function arguments' names"""

    def _make_plat_method_addr_regex(self, platform: BROMA_PLATFORMS) -> str:
        """Makes a platform method address regex

        Args:
            platform (BROMA_PLATFORMS): The platform

        Returns:
            str: RX_METHOD_PLAT_ADDR_BASE with the platform
        """
        return self.RX_METHOD_PLAT_ADDR_BASE.replace("{platform}", platform)

    def _parse_method_platforms(
        self,
        platforms: str | None
    ) -> dict[BROMA_PLATFORMS, int]:
        """Parses "win 0x13be40, imac 0x586990, m1 inline" to
        { "win": 0x13be40, "imac": 0x586990, "m1": 0 }

        Args:
            platforms (str | None): Platforms string of format "plat 0xaddr"

        Returns:
            dict[BROMA_PLATFORMS, int]: Platform-to-Address dict
        """
        if platforms is None:
            return {}

        platforms_list = [
            f"""{plat.split(" ")[0]} 0x0"""
            if plat.endswith("inline") else plat
            for plat in platforms.split(", ")
        ]

        return {
            cast(BROMA_PLATFORMS, plat_inf[0]): int(plat_inf[1], 16)
            for plat_inf in [
                plat_addr_pair.split(" ") for plat_addr_pair in platforms_list
            ]
        }

    def _get_broma_string(
            self,
            parsed_broma_line: Match[str],
            binding: Binding
    ) -> str:
        """Gets the Broma line to export after adding the binding's address

        Args:
            parsed_broma_line (Match[str]): Parsed Broma line in export()
            binding (Binding): The binding to add the address of

        Returns:
            str: The Broma line with the address
            (return type and function args names if applicable) added
        """
        # some very freaky code below (it uses nonlocal), you have been warned
        # TODO: literally do anything else bruh please

        sub_repl: str

        def sub_ret_type_f(m: Match[str]):
            nonlocal sub_repl

            groups = m.groups()

            if groups[0] == "":
                return "\t{} {}({}){addr}".replace(
                    "{addr}", f" = {groups[4]}" if groups[4] else ""
                ).format(
                    *(sub_repl, *groups[2:])
                )

            return "\t{} {} {}({}){addr}".replace(
                "{addr}", f" = {groups[4]}" if groups[4] else ""
            ).format(
                *(groups[0], sub_repl, *groups[2:])
            )

        def sub_args_names_f(m: Match[str]):
            nonlocal sub_repl

            groups = m.groups()

            if groups[0] == "":
                return "\t{ret}{}({}){addr}".replace(
                    "{ret}", f"{groups[1]} " if groups[1] else ""
                ).replace(
                    "{addr}", f" = {groups[4]}" if groups[4] else ""
                ).format(
                    groups[2], sub_repl
                )

            return "\t{} {ret}{}({}){addr}".replace(
                "{ret}", f"{groups[1]} " if groups[1] else ""
            ).replace(
                "{addr}", f" = {groups[4]}" if groups[4] else ""
            ).format(
                # step intentional; skip return type group
                *(*groups[:3:2], sub_repl)
            )

        self.num_exports += 1

        if DataManager().get("export_return_types") and (
                binding.ret.type not in (
                    "TodoReturn", "", parsed_broma_line.group(2)
                ) and
                # not ctor or dtor
                parsed_broma_line.group(2) is not None
        ):
            self.num_ret_exports += 1

            sub_repl = str(binding.ret)

            new_line = sub(
                self.RX_METHOD,
                sub_ret_type_f,
                parsed_broma_line.string
            )
            parsed_broma_line = search(
                self.RX_METHOD,
                new_line
            )  # type: ignore

        if DataManager().get("export_args_names"):
            broma_args = BEUtils.parse_str_args(
                parsed_broma_line.group(4),
                binding.class_name,
                binding.is_static
            )

            # pX is Broma auto-generated name, aX is IDA auto-generated name
            if any((
                match(r"(p|a)([0-9]+)", p.name) is None and
                p.name != broma_args[i].name
                for i, p in enumerate(binding.parameters)
            )):
                self.num_args_names_exports += 1

                sub_repl = binding.get_args_str(True, True)

                new_line = sub(
                    self.RX_METHOD,
                    sub_args_names_f,
                    parsed_broma_line.string
                )
                parsed_broma_line = search(
                    self.RX_METHOD,
                    new_line
                )  # type: ignore

        # freaky code ends

        broma_line_no_address = search(r"\);(.*)", parsed_broma_line.string)
        # binding has no address associated to it
        if broma_line_no_address:
            return f"""{
                broma_line_no_address.string[
                    :broma_line_no_address.span(0)[0]
                ]
            }) = {self._target_platform} {hex(binding.address)};{
                broma_line_no_address.string[
                    broma_line_no_address.span(0)[0]:
                ][2:]
            }"""

        broma_binding_platforms = self._parse_method_platforms(
            parsed_broma_line.group(5)
        )

        # binding has another platform's address associated to it
        if self._target_platform not in broma_binding_platforms:
            return sub(
                r"((?:(?:win|imac|m1|ios) (?:0[xX][0-9a-fA-F]+|inline)(?:, )?)+)",  # noqa: E501
                rf"""{self._target_platform} {hex(binding.address)}, \1""",
                parsed_broma_line.string
            )

        # binding has this platform's address associated to it
        if broma_binding_platforms[self._target_platform] != \
                binding.address:
            if AskPopup(
                "Mismatch in Broma for method "
                f"""{binding.qualified_name} """
                f"""(Broma: {
                    "inlined" if broma_binding_platforms[
                        self._target_platform
                    ] == 0 else hex(
                        broma_binding_platforms[self._target_platform]
                    )
                }. """
                f"""idb: {hex(binding.address)})\n"""
                "Overwrite Broma or Keep?",
                "Overwrite", "Keep",
                icon="WARNING"
            ).show() == ASKBTN_BTN1:
                return sub(
                    self._make_plat_method_addr_regex(self._target_platform),
                    f"{self._target_platform} "
                    f"""{hex(binding.address)}""",
                    parsed_broma_line.string
                )

        self.num_exports -= 1

        return parsed_broma_line.string

    def __init__(self, platform: BROMA_PLATFORMS, file: str):
        self._reset()
        self._target_platform = platform
        self._filepath = file

    def push_binding(self, binding: Binding):
        """Adds a binding to be exported

        Args:
            binding (Binding): The binding to be exported
        """
        self.bindings.update([
            (binding.qualified_name, binding)
        ])

    def push_bindings(self, bindings: list[Binding]):
        """Adds a list of bindings at once

        Args:
            bindings (list[Binding]): The list of bindings
        """
        self.bindings.update([
            (binding.qualified_name, binding)
            for binding in bindings
        ])

    def import_from_idb(self, names: Generator[tuple[int, str], Any, None]):
        """Imports bindings from an idautils.Names generator

        Args:
            names (Generator[tuple[int, str], Any, None]):
                The idautils.Names generator
        """
        for ea, name in names:
            if "::" not in name:
                continue

            split_name = name.split("::")

            # avoid exporting symbols more than 1 namespace/class deep
            # (is the case for cocos methods)
            if len(split_name) > 2:
                continue

            # ["GJUINode", "dGJUINode"] -> "GJUINode" == "GJUINode"
            if split_name[0] == split_name[1][1:]:
                split_name = [
                    split_name[0],
                    split_name[1].replace("::d", "::~")
                ]

            # check if first argument is ClassName*
            # because func_type_data_t.is_static is always False :D
            func_info = IDAUtils.get_function_info(ea, True)
            first_arg = next(iter(func_info), None) if func_info is not None \
                else None

            # i dont feel like it rn tbh
            if func_info is not None:
                if "std::" in str(func_info.rettype) or \
                        any(("std::" in str(p.type) for p in func_info)):
                    continue

            split_name_overload = search(r"(\S+)::(\S+)_[0-9]+", split_name[1])
            if split_name_overload is not None:
                original_name = f"""{
                    split_name_overload[0]
                }::{
                    split_name_overload[1]
                }"""

                self.overloads.setdefault(original_name, [])

                if original_name not in self.overloads:
                    original_overload = self.bindings.pop(original_name, None)

                    if original_overload is not None:
                        self.overloads[original_name].append(original_overload)

                self.overloads[original_name].append(Binding(
                    split_name[1],
                    split_name[0],
                    ea - get_imagebase(),
                    RetType(
                        str(
                            func_info.rettype if func_info else ""
                        ).replace(" *", "*").replace(" &", "&")
                    ),
                    BEUtils.from_ida_args(ea),
                    is_static=str(
                        first_arg.type if first_arg else ""
                    ) != f"{split_name[0]} *"
                ))

                continue

            self.push_binding(Binding(
                split_name[1],
                split_name[0],
                ea - get_imagebase(),
                RetType(
                    str(
                        func_info.rettype if func_info else ""
                    ).replace(" *", "*").replace(" &", "&")
                ),
                BEUtils.from_ida_args(ea),
                is_static=str(
                        first_arg.type if first_arg else ""
                    ) != f"{split_name[0]} *"
            ))

    def export(self):
        """Exports the bindings to the file"""
        if not path.exists(self._filepath):
            with open(self._filepath, "w"):
                pass

        with open(self._filepath, "r") as fr, \
                open(f"{self._filepath}.tmp", "w") as fw:
            current_class_name: str = ""

            for line in fr.readlines():
                if current_class_name == "":
                    current_class = match(self.RX_CLASS, line)

                    if current_class:
                        current_class_name = current_class.group(1)

                    fw.write(line)
                    continue

                if current_class_name == "":
                    fw.write(line)
                    continue

                if line.startswith("}"):
                    current_class_name = ""
                    fw.write(line)
                    continue

                func = search(self.RX_METHOD, line)
                if func is None:
                    fw.write(line)
                    continue

                qualified_name = f"{current_class_name}::{func.group(3)}"

                if qualified_name in self.overloads:
                    if not HAS_IDACLANG:
                        fw.write(line)
                        continue

                    for i, overload in enumerate(
                            self.overloads[qualified_name]
                    ):
                        if overload.has_same_args(
                                BEUtils.parse_str_args(
                                    func.group(4),
                                    current_class_name,
                                    func.group(1) == "static",
                                )
                        ):
                            fw.write(
                                self._get_broma_string(func, overload)
                            )
                            self.overloads[qualified_name].pop(i)
                            continue

                if qualified_name not in self.bindings:
                    fw.write(line)
                    continue

                if not self.bindings[qualified_name].has_same_args(
                        BEUtils.parse_str_args(
                            func.group(4),
                            current_class_name,
                            func.group(1) == "static"
                        )
                ):
                    fw.write(line)
                    continue

                fw.write(
                    self._get_broma_string(
                        func,
                        self.bindings[qualified_name]
                    )
                )

        move(f"{self._filepath}.tmp", self._filepath)

    def _reset(self):
        """Resets a BromaExporter instance"""
        self._filepath = ""
        self._target_platform = ""  # type: ignore
        self.bindings.clear()
        self.overloads.clear()
        self.num_exports = 0
