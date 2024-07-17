from typing import get_args, Union, NoReturn, Optional

from idaapi import (
    get_imagebase, get_file_type_name,
    BADADDR, SN_NOWARN
)
from ida_kernwin import ask_buttons, ask_str, ASKBTN_BTN1
from ida_name import get_name_ea
from idc import set_name

from sys import path as sys_path
from os import path as os_path
from pathlib import Path

from broma_ida.broma.constants import BROMA_PLATFORMS
from broma_ida.broma.binding import Binding


g_platform: Optional[BROMA_PLATFORMS] = None


def stop(reason: Optional[str] = None) -> NoReturn:
    """Nuh Uh"""
    raise Exception() if reason is None else Exception(reason)


def popup(
    button1: str,
    button2: Optional[str],
    button3: Optional[str],
    text: str,
    default: int = 1
) -> int:
    """Quick popup
    TODO: make it so only 1 button appears when option2 and option3 are empty
    ^ only possible using ida_kernwin.Forms, which honestly just sucks too much
    ass to even bother with

    Args:
        button1 (str): Button 1 text
        button2 (str): Button 2 text. None will hide the button inshallah.
        button3 (str): Button 3 text. None hides the button.
        text (str): The text to put.
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
                "Overwrite or keep current name?\n"
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
    return f"""{binding["qualified_name"]} @ {hex(binding["address"])}"""


def get_platform() -> Union[BROMA_PLATFORMS, NoReturn]:
    """Tries to get the binary's platform
    Returns:
        BROMA_PLATFORMS | NoReturn: The platform or stop() called if
            not able to be determined
    """
    global g_platform
    if g_platform is not None:
        return g_platform

    structure_info = get_file_type_name()

    if "PE" in structure_info:
        g_platform = "win"
        return "win"
    elif structure_info.endswith("ARM64"):
        g_platform = "m1"
        return "m1"
    elif structure_info.endswith("X86_64"):
        g_platform = "imac"
        return "imac"
    elif structure_info.startswith("ELF for ARM"):
        g_platform = "android32"
        return "android32"
    elif structure_info.startswith("ELF64 for ARM64"):
        g_platform = "android64"
        return "android64"

    platform = ask_str(
        "win",
        256,
        "Enter a platform (win, imac (intel mac), m1, "
        "ios, android32 (Android 32 bit) or android64 (Android 64 bit))"
    )

    if platform not in get_args(BROMA_PLATFORMS):
        popup(
            "Ok", None, None,
            f"""Invalid platform! ("{platform}" not in ("{
                get_args(BROMA_PLATFORMS)
            }\"))"""
        )
        stop()

    g_platform = platform

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
        "imac": "Intel MacOS",  # MacchewOS my beloved
        "m1": "M1 MacOS",
        "android32": "Android 32 bit",
        "android64": "Android 64 bit"
    }

    return platform_to_printable[platform]


def get_ida_plugin_path() -> Optional[Path]:
    """_summary_

    Returns:
        str: _description_
    """
    paths = [path for path in sys_path if "plugins" in path]

    if len(paths) == 0:
        return None

    return Path(
        os_path.abspath(os_path.join(os_path.dirname(paths[0]), "plugins"))
    )
