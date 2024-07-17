from typing import cast, Optional, Generator, Any
from os import path

from idaapi import get_imagebase

from re import search, match, sub, Match
from shutil import move

from broma_ida.broma.constants import BROMA_PLATFORMS
from broma_ida.broma.binding import Binding
from broma_ida.utils import popup


class BromaExporter:
    """Broma exporter of all time using regex (if you couldn't already tell)"""

    """
    group 1: class name
    """
    RX_CLASS = r"""class (\S+)"""

    """
    group 1: "virtual", "callback", "static", or empty
    group 2: return type
    group 3: function name
    group 4: comma-separated parameters or empty
    group 5: comma-separated platform address(es)
    """
    RX_METHOD = \
        r"""^(?:\t| {4})(?:\/\/ )?(virtual|callback|static|)(?: )?(?:(\S+) )?(\S+)\((.*)\)(?: const|)(?: = (?(?=inline).* \{|((?:(?:win|imac|m1|ios) (?:0[xX][0-9a-fA-F]+|inline)(?:, )?)+)))?"""  # noqa: E501
    RX_METHOD_PLAT_ADDR_BASE = r"""{platform} (0[xX][0-9a-fA-F]+|inline)"""

    _filepath: str = ""
    _target_platform: BROMA_PLATFORMS

    # { "qualified_name": Binding }
    bindings: dict[str, Binding] = {}
    num_exports: int = 0

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
        platforms: Optional[str]
    ) -> dict[BROMA_PLATFORMS, int]:
        """Parses "win 0x13be40, mac 0x586990, m1 inline" to
        { "win": 0x13be40, "mac": 0x586990, "m1": 0 }

        Args:
            platforms (str | None): Platforms string of format "plat 0xaddr"

        Returns:
            dict[BROMA_PLATFORMS, int]: Platform-to-Address dict
        """
        if platforms is None:
            return {}
        
        platforms_list = platforms.split(", ")
        platforms_list = [f"""{plat.split(" ")[0]} 0x0""" if plat.endswith("inline") else plat for plat in platforms_list]

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
        """Get's the Broma line to export after adding the binding's address

        Args:
            parsed_broma_line (Match[str]): Parsed Broma line in export()
            binding (Binding): The binding to add the address of

        Returns:
            str: The Broma line with the address added
        """
        self.num_exports += 1

        broma_line_no_address = search(r"\);(.*)", parsed_broma_line.string)
        # binding has no address associated to it
        if broma_line_no_address:
            return f"""{
                broma_line_no_address.string[
                    :broma_line_no_address.span(0)[0]
                ]
            }) = {self._target_platform} {hex(binding["address"])} {
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
                r"((?:(?:win|imac|m1|ios) (?:0[xX][0-9a-fA-F]+|inline)(?:, )?)+)",
                rf"""{self._target_platform} {hex(binding["address"])}, \1""",
                parsed_broma_line.string
            )

        # binding has this platform's address associated to it
        if broma_binding_platforms[self._target_platform] != \
                binding["address"]:
            if popup(
                "Overwrite", "Keep", None,
                "Mismatch in Broma for method "
                f"""{binding["qualified_name"]} """
                f"""(Broma: {
                    "inlined" if broma_binding_platforms[
                        self._target_platform
                    ] == 0 else hex(
                        broma_binding_platforms[self._target_platform]
                    )
                }. """
                f"""idb: {hex(binding["address"])})\n"""
                "Overwrite Broma or Keep?"
            ) == 1:
                return sub(
                    self._make_plat_method_addr_regex(self._target_platform),
                    f"{self._target_platform} "
                    f"""{hex(binding["address"])}""",
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
            (binding["qualified_name"], binding)
        ])

    def push_bindings(self, bindings: list[Binding]):
        """Adds a list of bindings at once

        Args:
            bindings (list[Binding]): The list of bindings
        """
        self.bindings.update([
            (binding["qualified_name"], binding)
            for binding in bindings
        ])

    def import_from_idb(self, names: Generator[tuple[Any, Any], Any, None]):
        """Imports bindings from an idautils.Names generator

        Args:
            names (Generator[tuple[Any, Any], Any, None]):
                The idautils.Names generator
        """
        for ea, name in names:
            if "::" not in name:
                continue

            split_name = name.split("::")

            # ["GJUINode", "dGJUINode"] -> "GJUINode" == "GJUINode"
            if split_name[0] == split_name[1][1:]:
                split_name = [
                    split_name[0],
                    split_name[1].replace("::d", "::~")
                ]

            self.push_binding(Binding({
                "name": split_name[1],
                "class_name": split_name[0],
                "address": ea - get_imagebase()
            }, search(r"\S+::\S+_[0-9]+$", name) is not None))

    def export(self):
        """Exports the bindings to the file
        TODO: handle function overloads"""
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

                if f"{current_class_name}::{func.group(3)}" not in \
                        self.bindings:
                    fw.write(line)
                    continue

                binding = self.bindings[
                    f"{current_class_name}::{func.group(3)}"
                ]

                fw.write(self._get_broma_string(func, binding))

        move(f"{self._filepath}.tmp", self._filepath)

    def _reset(self):
        """Resets a BromaExporter instance"""
        self._filepath = ""
        self._target_platform = ""  # type: ignore
        self.bindings.clear()
        self.num_exports = 0
