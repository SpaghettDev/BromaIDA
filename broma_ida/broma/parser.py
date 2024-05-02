from io import TextIOWrapper

from broma_ida.broma.constants import BROMA_PLATFORMS
from broma_ida.binding_type import Binding

from re import match


class BromaParser:
    """Broma parser of all time using regex"""
    RX_CLASS = r"""class (\S+)( : (.*))? \{"""
    RX_METHOD_BASE = \
        r"""(?:\t| {4})(virtual|)(.* )?(\S+)\((.*)\)(?: const|) = .*{platform} (0[xX][0-9a-fA-F]+)"""  # noqa: E501

    _target_platform: BROMA_PLATFORMS

    bindings: list[Binding] = []
    # { "address": [Binding, Binding, ...] }
    duplicates: dict[int, list[Binding]] = {}

    def _make_plat_method_regex(self, platform: BROMA_PLATFORMS) -> str:
        """Makes a platform regex

        Args:
            platform (BROMA_PLATFORMS):
                platform name (win, mac or ios :trollface:)

        Returns:
            str: RX_METHOD_BASE with the platform
        """
        return self.RX_METHOD_BASE.replace("{platform}", platform)

    def _get_binding_of_address(self, addr: int) -> Binding:  # type: ignore
        """There are at most 1 binding in bindings_list,
        so this will always return 1 binding (not None since
        we never fabricate a binding (hopefully)

        Args:
            addr (int): Address of binding to look for

        Returns:
            Binding: The binding associated with that address
        """
        for b in self.bindings:
            if addr == b["address"]:
                return b

    def __init__(self, platform: BROMA_PLATFORMS):
        """Initializes a BromaParser instance for a specific platform

        Args:
            platform (BROMA_PLATFORMS): Target platform
        """
        self._target_platform = platform

    def parse_file_stream(self, file: TextIOWrapper):
        """Parses a .bro file from a file stream

        Args:
            file (TextIOWrapper): The file stream

        Returns:
            tuple[list[Binding], dict[int, list[Binding]]]:
            0 contains unique bindings, 1 contains duplicates
        """
        current_class_name: str = ""
        current_class_inheritance: list[str] = []
        method_regex = self._make_plat_method_regex(self._target_platform)

        for line in file.readlines():
            if current_class_name == "":
                current_class = match(self.RX_CLASS, line)
                if current_class:
                    current_class_name = current_class.group(1)

                    if current_class.group(2) is not None:
                        current_class_inheritance = \
                            current_class.group(2).lstrip(" : ").split(", ")

                continue

            if line.startswith("}"):
                current_class_name = ""
                current_class_inheritance.clear()
                continue

            func = match(method_regex, line)
            if func is None:
                continue

            func_address = int(func.group(5), 16)
            func_name = func.group(3)

            # Runs only for the first time an address has a duplicate
            if func_address in self.bindings:
                dup_binding = self._get_binding_of_address(func_address)
                error_location = \
                    f"{current_class_name}::{func_name} " \
                    f"and {dup_binding['qualifiedName']} " \
                    f"@ {hex(func_address)}"

                if f"{current_class_name}::{func_name}" == \
                        dup_binding['qualifiedName']:
                    print(
                        "[!] Duplicate binding with same qualified name! "
                        f"({error_location})"
                    )
                    continue
                elif current_class_name == dup_binding['className']:
                    print(
                        "[!] Duplicate binding within same class! "
                        f"({error_location})"
                    )
                    continue

                print(
                    "[!] Duplicate binding! "
                    f"({current_class_name}::{func_name} "
                    f"and {dup_binding['qualifiedName']} "
                    f"@ {hex(func_address)})"
                )
                del self.bindings[self.bindings.index(dup_binding)]
                self.duplicates[func_address] = []
                self.duplicates[func_address].append(dup_binding)

            if func_address in self.duplicates:
                self.duplicates[func_address].append(Binding({
                    "name": func_name,
                    "className": current_class_name,
                    "inheritedClasses": current_class_inheritance[:],
                    "address": func_address
                }))
                continue

            # print(f"{current_class_name}::{func_name} = {hex(func_address)}")

            self.bindings.append(Binding({
                "name": func_name,
                "className": current_class_name,
                "inheritedClasses": current_class_inheritance[:],
                "address": func_address
            }))

        file.close()

    def reset(self):
        """Resets a BromaParser instance"""
        self._target_platform = ""  # type: ignore
        self.bindings.clear()
        self.duplicates.clear()
