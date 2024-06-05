from typing import get_args, Union, NoReturn, Optional

from idaapi import (
    get_imagebase, get_inf_structure, get_file_type_name,
    BADADDR, SN_NOWARN,
    f_PE, f_MACHO
)
from ida_kernwin import ask_buttons, ask_str, ASKBTN_BTN1
from ida_name import get_name_ea
from idc import set_name

from sys import path as sys_path
from os import path as os_path
from re import sub
from pathlib import Path

from broma_ida.broma.constants import (
    BROMA_PLATFORMS, CPP_TYPE_QUALIFIERS, CPP_DATA_TYPES,
    CPP_PRIMITIVES
)
from broma_ida.broma.binding import Binding
from broma_ida.broma.argtype import ArgType


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

    structure_info = get_inf_structure()

    if structure_info.filetype == f_PE:
        g_platform = "win"
        return "win"

    if structure_info.filetype == f_MACHO:
        file_type_name = get_file_type_name()

        if file_type_name.endswith("ARM64"):
            g_platform = "ios"
            return "ios"
        elif file_type_name.endswith("X86_64"):
            g_platform = "mac"
            return "mac"

    if g_platform == "":
        platform = ask_str(
            "win", 256, "Enter a platform (win, mac or ios)"
        )
        g_platform = platform

    if platform not in get_args(BROMA_PLATFORMS):
        popup(
            "Ok", None, None,
            f"""Invalid platform! ("{platform}" not in ("{
                get_args(BROMA_PLATFORMS)
            }\"))"""
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


def are_args_primitive(arguments: dict[str, ArgType]) -> bool:
    """Checks if the arguments are all primitives because
    importing structs isn't supported yet
    TODO: start importing structs :D

    Args:
        arguments (dict[str, str])

    Returns:
        bool: ya
    """
    is_pointer_or_reference = \
        lambda p: p[-1] == "*" or p[-1] == "&"  # noqa: E731
    remove_pointer_or_reference = \
        lambda p: p[:-1] if is_pointer_or_reference(p) else p  # noqa: E731

    if len(arguments) == 0:
        return True

    arguments_str = {
        name: t["type"] for name, t in arguments.items()
    }

    for name, arg_type in arguments_str.items():
        arguments_str[name] = sub(
            rf"""(?: )?unsigned(?!{
                "|".join([f" {x}" for x in CPP_DATA_TYPES])
            })""",
            "unsigned int",
            arg_type
        )

        for qualifier in CPP_TYPE_QUALIFIERS:
            arguments_str[name] = sub(
                rf"(?: )?{qualifier}(?: )?",
                "",
                arguments_str[name]
            )

        arguments_str[name] = remove_pointer_or_reference(arguments_str[name])

    return all([
        arg_type in CPP_PRIMITIVES for _, arg_type in arguments_str.items()
    ])


def is_primitive_type(ret: str) -> bool:
    """Is a type a C++ primitive

    Args:
        ret (str): ya

    Returns:
        bool: ya
    """
    return ret in CPP_PRIMITIVES


def get_ida_plugin_path() -> Optional[Path]:
    """_summary_

    Returns:
        str: _description_
    """
    paths = [path for path in sys_path if "plugins" in path]

    if len(paths) != 1:
        return None

    return Path(
        os_path.abspath(os_path.join(os_path.dirname(paths[0]), "plugins"))
    )
