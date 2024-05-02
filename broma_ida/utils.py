from typing import get_args, NoReturn, Union

from idaapi import (
    get_imagebase, get_inf_structure, get_file_type_name,
    BADADDR, SN_NOWARN,
    f_PE, f_MACHO
)
from ida_kernwin import ask_buttons, ask_str, ASKBTN_BTN1
from ida_name import get_name_ea
from idc import set_name

from broma_ida.binding_type import Binding
from broma_ida.broma.constants import BROMA_PLATFORMS


def stop(reason: Union[str, None] = None) -> NoReturn:
    """Nuh Uh"""
    raise Exception() if reason is None else Exception(reason)


def popup(
    button1: str,
    button2: Union[str, None],
    button3: Union[str, None],
    text: str,
    default: int = 1
) -> int:
    """Quick popup
    TODO: make it so only 1 button appears when option2 and option3 are empty
    ^ only possible using ida_kernwin.Forms, which honestly just sucks too much
    ass to even bother with

    Args:
        button1 (str): Button 1 text
        button2 (str): Button 2 text
        button3 (str): Button 3 text
        text (str): The text to put int the popup
        default (int, optional): Default option. Defaults to 1.
    """
    return ask_buttons(
        button1,
        "" if button2 is None else button2,
        "" if button3 is None else button3,
        default,
        f"HIDECANCEL\n{text}" if button3 is None else text
    )


def rename_func(addr: int, name: str, max: int = 10) -> bool:
    """Renames the addr. Accounts for overloads by appending _X
    where X is a number between 1 and max (exclusive)

    Args:
        addr (int): The address to rename
        name (str): The name to give it
        max (int, optional): Maximum number of retires. Defaults to 10.

    Returns:
        bool: True if the address has been renamed successfully
        after max trues
    """
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
                f"sub_{hex(ida_prev_addr)[2:].upper()})"
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


def get_platform() -> Union[BROMA_PLATFORMS, NoReturn]:
    """Tries to get the binary's platform
    Returns:
        BROMA_PLATFORMS | None: The platform or None if
            not able to be determined
    """
    structure_info = get_inf_structure()

    if structure_info.filetype == f_PE:
        return "win"

    if structure_info.filetype == f_MACHO:
        file_type_name = get_file_type_name()

        if file_type_name.endswith("ARM64"):
            return "ios"
        elif file_type_name.endswith("X86_64"):
            return "mac"

    platform = ask_str(
        "win", 256, "Enter a platform (win, mac or ios)"
    )

    if platform not in get_args(BROMA_PLATFORMS):
        popup(
            "Ok", None, None,
            f"""Invalid platform! ({platform} not "win", "mac" or "ios")"""
        )
        stop()

    return platform


def get_platform_printable(platform: BROMA_PLATFORMS) -> str:
    """Printable platform name

    Args:
        platform (BROMA_PLATFORMS): The platform

    Returns:
        str: Printable name of said platform
    """
    platform_to_printable = {
        "win": "Windows",
        "ios": "iOS",
        "mac": "MacOS"  # MacchewOS my beloved
    }

    return platform_to_printable[platform]