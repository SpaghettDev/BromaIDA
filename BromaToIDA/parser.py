from io import TextIOWrapper
from BindingType import Binding

from re import search


RX_CLASS = r"""class (\S+)( : (.*))? \{"""
RX_WIN_METHOD = \
    r"""(?:\t| {4})(virtual|)(.* )?(\S+)\((.*)\) = win (0[xX][0-9a-fA-F]+)"""


# TODO: make it a class?
def parse_broma(
    file: TextIOWrapper
) -> tuple[list[Binding], dict[int, list[Binding]]]:
    """Parses a .bro file

    Args:
        file (TextIOWrapper): The file stream

    Returns:
        tuple[list[Binding], dict[int, list[Binding]]]:
        0 contains unique bindings, 1 contains duplicates
    """
    def get_binding_of_address(
        bindings_list: list[Binding], addr: int
    ) -> Binding:  # type: ignore
        """There are at most 1 binding in bindings_dups_tup,
        so this will always return 1 binding (not None since
        we never fabricate a binding (hopefully)
        """
        for b in bindings_list:
            if addr == b["address"]:
                return b

    current_class_name = None
    current_class_inheritance: list[str] = []
    bindings_list: list[Binding] = []
    duplicate_dict: dict[int, list[Binding]] = {}
    in_class = False

    for line in file.readlines():
        if current_class_name is None:
            current_class = search(RX_CLASS, line)
            if current_class:
                current_class_name = current_class.group(1)

                if current_class.group(2) is not None:
                    current_class_inheritance = \
                        current_class.group(2).lstrip(" : ").split(", ")

            continue

        if current_class_name is not None and not in_class:
            in_class = True

        if line.startswith("}"):
            current_class_name = None
            current_class_inheritance.clear()
            in_class = False
            continue

        func = search(RX_WIN_METHOD, line)

        if func is None:
            continue

        func_address = int(func.group(5), 16)
        func_name = func.group(3)

        if func_address in bindings_list:
            dup_binding = get_binding_of_address(bindings_list, func_address)
            error_location = \
                f"({current_class_name}::{func_name} " \
                f"and {dup_binding['qualifiedName']} " \
                f"@ {hex(func_address)})"

            if f"{current_class_name}::{func_name}" == \
                    dup_binding['qualifiedName']:
                print(
                    "[!] Duplicate binding with same qualified name! "
                    f"{error_location}"
                )
                continue
            elif current_class_name == dup_binding['className']:
                print(
                    "[!] Duplicate binding within same class! "
                    f"{error_location}"
                )
                continue

            print(
                f"[!] Duplicate binding! ({current_class_name}::{func_name} "
                f"and {dup_binding['qualifiedName']} "
                f"@ {hex(func_address)})"
            )
            del bindings_list[bindings_list.index(dup_binding)]
            duplicate_dict[func_address] = []
            duplicate_dict[func_address].append(dup_binding)

        if func_address in duplicate_dict:
            duplicate_dict[func_address].append(Binding({
                "name": func_name,
                "className": current_class_name,
                "inheritedClasses": current_class_inheritance[:],
                "address": func_address
            }))
            continue

        # print(f"{current_class_name}::{func_name} = {hex(func_address)}")

        bindings_list.append(Binding({
            "name": func_name,
            "className": current_class_name,
            "inheritedClasses": current_class_inheritance[:],
            "address": func_address
        }))

    return bindings_list, duplicate_dict

from io import TextIOWrapper
from BindingType import Binding

from re import search


RX_CLASS = r"""class (\S+)( : (.*))? \{"""
RX_WIN_METHOD = \
    r"""(?:\t| {4})(virtual|)(.* )?(\S+)\((.*)\) = win (0[xX][0-9a-fA-F]+)"""


# TODO: make it a class?
def parse_broma(
    file: TextIOWrapper
) -> tuple[list[Binding], dict[int, list[Binding]]]:
    """Parses a .bro file

    Args:
        file (TextIOWrapper): The file stream

    Returns:
        tuple[list[Binding], dict[int, list[Binding]]]:
        0 contains unique bindings, 1 contains duplicates
    """
    def get_binding_of_address(
        bindings_list: list[Binding], addr: int
    ) -> Binding:  # type: ignore
        """There are at most 1 binding in bindings_dups_tup,
        so this will always return 1 binding (not None since
        we never fabricate a binding (hopefully)
        """
        for b in bindings_list:
            if addr == b["address"]:
                return b

    current_class_name = None
    current_class_inheritance: list[str] = []
    bindings_list: list[Binding] = []
    duplicate_dict: dict[int, list[Binding]] = {}
    in_class = False

    for line in file.readlines():
        if current_class_name is None:
            current_class = search(RX_CLASS, line)
            if current_class:
                current_class_name = current_class.group(1)

                if current_class.group(2) is not None:
                    current_class_inheritance = \
                        current_class.group(2).lstrip(" : ").split(", ")

            continue

        if current_class_name is not None and not in_class:
            in_class = True

        if line.startswith("}"):
            current_class_name = None
            current_class_inheritance.clear()
            in_class = False
            continue

        func = search(RX_WIN_METHOD, line)

        if func is None:
            continue

        func_address = int(func.group(5), 16)
        func_name = func.group(3)

        if func_address in bindings_list:
            dup_binding = get_binding_of_address(bindings_list, func_address)
            error_location = \
                f"({current_class_name}::{func_name} " \
                f"and {dup_binding['qualifiedName']} " \
                f"@ {hex(func_address)})"

            if f"{current_class_name}::{func_name}" == \
                    dup_binding['qualifiedName']:
                print(
                    "[!] Duplicate binding with same qualified name! "
                    f"{error_location}"
                )
                continue
            elif current_class_name == dup_binding['className']:
                print(
                    "[!] Duplicate binding within same class! "
                    f"{error_location}"
                )
                continue

            print(
                f"[!] Duplicate binding! ({current_class_name}::{func_name} "
                f"and {dup_binding['qualifiedName']} "
                f"@ {hex(func_address)})"
            )
            del bindings_list[bindings_list.index(dup_binding)]
            duplicate_dict[func_address] = []
            duplicate_dict[func_address].append(dup_binding)

        if func_address in duplicate_dict:
            duplicate_dict[func_address].append(Binding({
                "name": func_name,
                "className": current_class_name,
                "inheritedClasses": current_class_inheritance[:],
                "address": func_address
            }))
            continue

        # print(f"{current_class_name}::{func_name} = {hex(func_address)}")

        bindings_list.append(Binding({
            "name": func_name,
            "className": current_class_name,
            "inheritedClasses": current_class_inheritance[:],
            "address": func_address
        }))

    return bindings_list, duplicate_dict
