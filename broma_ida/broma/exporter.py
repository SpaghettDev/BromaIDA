from typing import cast, Union, Generator, Any
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
    group 4: comma-separated parameters  or empty
    group 5: comma-separated platform address(es)
    """
    RX_METHOD = \
        r"""^(?:\t| {4})(?:\/\/ )?(virtual|callback|static|)(?: )?(?:(.*) )?(\S+)\((.*)\)(?: const|)(?: = ((?:(?:win|mac|ios) (?:0[xX][0-9a-fA-F]+)(?:, )?)+))?;"""  # noqa: E501
    RX_METHOD_PLAT_ADDR_BASE = r"""{platform} (0[xX][0-9a-fA-F]+)"""

    _filepath: str = ""
    _target_platform: BROMA_PLATFORMS

    # { "qualifiedName": Binding }
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
        platforms: Union[str, None]
    ) -> dict[BROMA_PLATFORMS, int]:
        """Parses "win 0x13be40, mac 0x586990" to
        { "win": 0x13be40, "mac": 0x586990 }

        Args:
            platforms (str | None): Platforms string of format "plat 0xaddr"

        Returns:
            dict[BROMA_PLATFORMS, int]: Platform-to-Address dict
        """
        if platforms is None:
            return {}

        return {
            cast(BROMA_PLATFORMS, plat_inf[0]): int(plat_inf[1], 16)
            for plat_inf in [
                plat_addr.split(" ") for plat_addr in platforms.split(", ")
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
        # function is inlined
        if parsed_broma_line.string.endswith("{\n"):
            return parsed_broma_line.string

        self.num_exports += 1

        # binding has no address associated to it
        # TODO: this doesn't add binding to `...); // = idfk`
        if parsed_broma_line.string.endswith(");\n"):
            # strip last two characters (";\n") then add the binding's address
            return f"""{
                parsed_broma_line.string[:-2]
            } = {
                self._target_platform
            } {hex(binding["address"])};\n"""  # type: ignore

        broma_binding_platforms = self._parse_method_platforms(
            parsed_broma_line.group(5)
        )

        # binding has another platform's address associated to it
        if self._target_platform not in broma_binding_platforms:
            return sub(
                r"(?:(0[xX][0-9a-fA-F]+);)",
                rf"""\1, {self._target_platform} {hex(binding["address"])};""",  # type: ignore
                parsed_broma_line.string, 1
            )

        # binding has this platform's address associated to it
        if broma_binding_platforms[self._target_platform] != \
                binding["address"]:
            if popup(
                "Overwrite", "Keep", None,
                "Mismatch in Broma for method "
                f"""{binding["qualifiedName"]} """  # type: ignore
                f"""(Broma: {
                    hex(broma_binding_platforms[self._target_platform])
                }. """
                f"""idb: {hex(binding["address"])})\n"""  # type: ignore
                "Overwrite Broma or Keep?"
            ) == 1:
                return sub(
                    self._make_plat_method_addr_regex(self._target_platform),
                    f"{self._target_platform} "
                    f"""{hex(binding["address"])}""",  # type: ignore
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
            (binding["qualifiedName"], binding)  # type: ignore
        ])

    def push_bindings(self, bindings: list[Binding]):
        """Adds a list of bindings at once

        Args:
            bindings (list[Binding]): The list of bindings
        """
        self.bindings.update([
            (binding["qualifiedName"], binding)  # type: ignore
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
                "className": split_name[0],
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

                func = match(self.RX_METHOD, line)
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
