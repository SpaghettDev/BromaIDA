VERSION = "1.0.0"
__AUTHOR__ = "SpaghettDev"

PLUGIN_NAME = "Broma to IDA Importer (real)"

from idaapi import get_imagebase, BADADDR, SN_NOWARN
from ida_kernwin import (
    ask_file, ask_buttons, del_hotkey,
    add_hotkey, ASKBTN_BTN1, ASKBTN_BTN1, ASKBTN_CANCEL
)
from ida_name import get_name_ea
from ida_funcs import add_func, get_func
from idc import (
    get_name, set_name, get_func_flags, get_func_cmt,
    set_func_cmt, FUNC_LIB
)

from typing import NoReturn, Union, Callable, Union, TypedDict, Literal, cast

from io import TextIOWrapper
from re import sub, search


# legacy type
class BaseBindingType(TypedDict):
    """Base binding type"""
    name: str
    className: str
    qualifiedName: str
    idaQualifiedName: str
    inheritedClasses: list[str]
    address: int


class BaseShortBindingType(TypedDict):
    """Base binding type 2"""
    name: str
    className: str
    inheritedClasses: list[str]
    address: int


class Binding:
    """Actual binding type. Implements __eq__ because
    TypedDict can't be instantiated, and as such, can't
    have overloaded methods"""
    binding: BaseBindingType

    def __init__(
        self,
        binding: Union[BaseShortBindingType, BaseBindingType]
    ) -> None:
        if binding.get("qualifiedName"):
            self.binding = cast(BaseBindingType, binding)
        else:
            binding = cast(BaseShortBindingType, binding)

            self.binding = BaseBindingType({
                "name": binding["name"],
                "className": binding["className"],
                "qualifiedName":
                    f"""{binding["className"]}::{binding["name"]}""",
                "idaQualifiedName":
                    f"""{binding["className"]}::{binding["name"]}""".replace(
                        "::~", "::d"
                    ),
                "inheritedClasses": binding["inheritedClasses"],
                "address": binding["address"]
            })

    def __eq__(self, key: object) -> bool:
        if isinstance(key, int):
            return self.binding["address"] == key
        elif isinstance(key, str):
            return self.binding["qualifiedName"] == key

        return False

    def __getitem__(
        self,
        key: Literal[
            "name", "className", "qualifiedName",
            "idaQualifiedName", "inheritedClasses", "address"
        ]
    ) -> Union[str, list[str], int]:
        return self.binding.__getitem__(key)  # type: ignore

    def __str__(self) -> str:
        return f"""name="{self.binding["name"]}" """ \
            f"""className="{self.binding["className"]}" """ \
            f"""qualifiedName="{self.binding["qualifiedName"]}" """ \
            f"""idaQualifiedName="{self.binding["idaQualifiedName"]}" """ \
            f"""inheritedClasses="{self.binding["inheritedClasses"]}" """ \
            f"""address="{self.binding["address"]}\""""

def stop() -> NoReturn:
    """Nuh Uh"""
    raise SystemExit


def popup(
    button1: str,
    button2: Union[str, None],
    button3: Union[str, None],
    text: str,
    default: int = 1
) -> int:
    """Quick popup
    TODO: make it so only 1 button appears when option2 and option3 are empty
    ion think thats possible ^

    Args:
        button1 (str): Button 1 text
        button2 (str): Button 2 text
        button3 (str): Button 3 text
        text (str): The text to put int the popup
        default (int, optional): Default option. Defaults to 0.
    """
    return ask_buttons(
        button1,
        "" if button2 is None else button2,
        "" if button3 is None else button3,
        default,
        f"HIDECANCEL\n{text}" if button3 is None else text
    )


def resolve_inheritance(bindings: list[Binding]) -> list[str]:
    """Common inherited classes between a list of bindings.
    Disregards elements that end with "Delegate" or "Protocol".

    Args:
        l (list[Binding]): List of duplicates to resolve inheritance of.

    Returns:
        list[str]: List of inherited classes
    """
    inherited_classes = []

    for binding in bindings:
        for inherited_class in binding["inheritedClasses"]:  # type: ignore
            if not inherited_class.endswith("Delegate") or \
                    not inherited_class.endswith("Protocol"):
                inherited_classes.append(inherited_class)

    return inherited_classes


def rename_func(addr: int, name: str, max: int = 10) -> bool:
    renamed = False

    for i in range(max):
        if set_name(addr, name if i == 0 else f"{name}_{i}", SN_NOWARN):
            renamed = True
            break

    if not renamed:
        ida_prev_addr = get_name_ea(BADADDR, name)
        if ida_prev_addr != 0xFFFFFFFF and addr != ida_prev_addr:
            if popup(
                "Overwrite", "Keep", None,
                f"{name} is already taken at "
                f"{hex(ida_prev_addr - get_imagebase())} while trying to "
                f"rename {hex(addr)}\n"
                f"Overwrite or keep current name?\n"
                "(Old location will be renamed to "
                f"sub_{hex(ida_prev_addr)[2:]})"
            ) == ASKBTN_BTN1:
                set_name(
                    ida_prev_addr,
                    f"sub_{hex(ida_prev_addr)[2:]}",
                    SN_NOWARN
                )

    return renamed


def get_short_info(binding: Binding) -> str:
    """Short info about binding ([binding name] @ [binding address])

    Args:
        binding (Binding): The binding

    Returns:
        str: In the format of "[binding name] @ [binding address]"
    """
    return f"""{
        binding["qualifiedName"]
    } @ {hex(binding["address"])}"""  # type: ignore


def register_btida(hotkey: str, f: Callable):
    """Registers the main function's hotkey
    i couldn't figure out how to use ida_idaapi.plugin_t
    (partly because NOTHING FUCKING WORKS)
    so i resorted to this :D"""
    try:
        hotkey_ctx  # type: ignore
        if del_hotkey(hotkey_ctx):  # type: ignore
            print("[+] Hotkey unregistered!")
            del hotkey_ctx  # type: ignore
        else:
            print("[!] Failed to delete hotkey!")
    except:
        hotkey_ctx = add_hotkey(hotkey, f)
        if hotkey_ctx is None:
            print("[!] Failed to register hotkey!")
            del hotkey_ctx
        else:
            print("[+] Hotkey registered!")


RX_CLASS = r"""class (\S+)( : (.*))? \{"""
RX_WIN_METHOD = \
    r"""(?:\t| {4})(virtual|)(.* )?(\S+)\((.*)\) = win (0[xX][0-9a-fA-F]+)"""


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


def main():
    if popup("Ok", None, None, "Select the Broma file to import") != 0:
        filePath = ask_file(False, "GeometryDash.bro", "bro")

        if filePath is None or (filePath and not filePath.endswith(".bro")):
            popup("Ok", None, None, "Please select a valid file!")
            stop()

        parsed: tuple[list[Binding], dict[int, list[Binding]]]

        try:
            with open(filePath, "r") as f:
                parsed = parse_broma(f)
        except FileNotFoundError:
            popup("Ok", None, None, "File doesn't exist? Please try again.")
            stop()

        print(
            f"\n\n[+] Read {len(parsed[0])} Windows bindings from {filePath}"
        )
        print(
            f"[+] Read {len(parsed[1])} duplicate Windows bindings from "
            f"{filePath}\n"
        )

        # first, handle non-duplicates
        for binding in parsed[0]:
            ida_addr = get_imagebase() + binding["address"]
            ida_name = get_name(ida_addr)
            ida_func_flags = get_func_flags(ida_addr)

            if ida_name.startswith("loc_"):
                add_func(ida_addr)

            if ida_func_flags & FUNC_LIB:
                print(
                    f"""[!] Tried to rename a library function! """
                    f"({get_short_info(binding)})"
                )
                continue

            if get_func(ida_addr).start_ea != ida_addr:
                print(
                    f"""[!] Function is in the middle of another one! """
                    f"({get_short_info(binding)})"
                )
                continue

            if ida_name.startswith("sub_"):
                rename_func(
                    ida_addr,
                    binding["idaQualifiedName"]  # type: ignore
                )
            elif sub("_[0-9]+", "", ida_name) != binding["idaQualifiedName"]:
                mismatch_popup = popup(
                    "Overwrite", "Keep", "",
                    f"""Mismatch in broma ({binding["qualifiedName"]}) """
                    f"""and idb ({ida_name})!\n"""
                    "Overwrite from broma or keep current name?"
                )

                if mismatch_popup == ASKBTN_BTN1:
                    rename_func(
                        ida_addr,
                        binding["idaQualifiedName"]  # type: ignore
                    )
                elif mismatch_popup == ASKBTN_CANCEL:
                    stop()

        # and now for what took me 3 hours :D
        for addr, bindings in parsed[1].items():
            ida_addr = get_imagebase() + addr

            func_cmt = get_func_cmt(ida_addr, True)
            func_names = ", ".join(
                [binding["qualifiedName"] \
                    for binding in bindings]  # type: ignore
            )

            if func_cmt == "":
                # use the first occurrence as the name (very good imo)
                rename_func(
                    ida_addr,
                    bindings[0]["idaQualifiedName"]  # type: ignore
                )

                set_func_cmt(ida_addr, f"Merged with: {func_names}", True)
            elif func_cmt.startswith("Merged with: "):
                cmt_func_names = func_cmt.lstrip("Merged with: ")

                if func_names != cmt_func_names:
                    print(
                        "[!] Mismatch in merged function list "
                        f"(Current: {cmt_func_names}, Correct: {func_names})! "
                        "Correcting..."
                    )
                    set_func_cmt(ida_addr, f"Merged with: {func_names}", True)
            else:
                if popup(
                    "Overwrite", "Keep", None,
                    f"{hex(addr)} already has a comment! Would you like to "
                    "overwrite it with merge information or keep the current "
                    "comment?\n"
                    "(You will be prompted with this again if you rerun the "
                    "script and there are merged functions!)"
                ) == ASKBTN_BTN1:
                    set_func_cmt(ida_addr, f"Merged with: {func_names}", True)

        print("[+] Finished importing bindings from broma file")
        popup(
            "Ok", "Ok", None,
            "Finished importing Windows bindings from broma file."
        )


register_btida("Ctrl-Shift-I", main)
