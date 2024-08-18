from typing import NoReturn, Optional

from idaapi import get_imagebase, get_inf_structure, BADADDR, SN_NOWARN
from ida_kernwin import ask_buttons, ASKBTN_BTN1
from ida_name import get_name_ea
from ida_diskio import idadir
from ida_ida import f_PE, f_MACHO, f_ELF
from ida_segment import get_first_seg
from ida_bytes import get_dword, get_bytes
from ida_segment import get_segm_by_sel
from idc import set_name, selector_by_name

from struct import unpack
from pathlib import Path

from broma_ida.broma.constants import BROMA_PLATFORMS
from broma_ida.broma.binding import Binding


# Mach-O Load commands
MINIMUM_OS_VERSION_LOAD_COMMAND = 0x32

# Mach-O CPU types
CPU_TYPE_ARM64 = 0x0100000c
CPU_TYPE_X86_64 = 0x01000007

# Mach-O Platform types
PLATFORM_TYPE_MACOS = 0x1
PLATFORM_TYPE_IOS = 0x2

g_platform: BROMA_PLATFORMS = None  # type: ignore


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


# Internal
def _get_minimum_mach_o_os_version() -> int:
    """Gets the Minimum OS Version struct from the Mach-O header

    Returns:
        int: -1 if it couldn't find MOSV load command
    """
    start = get_imagebase()
    magic = get_dword(start)

    if magic == 0xFEEDFACF:
        header_size = 32  # 64-bit Mach-O header size
    else:
        header_size = 28  # 32-bit Mach-O header size

    mach_header = get_bytes(start, header_size)
    magic_number, cpu_type, cpu_subtype, file_type, \
        ncmds, cmds_size, flags, reserved = unpack("<IIIIIIII", mach_header)

    offset = start + header_size

    for _ in range(ncmds):
        cmd_header = get_bytes(offset, 8)
        if not cmd_header or len(cmd_header) < 8:
            break

        cmd, cmdsize = unpack("<II", cmd_header)

        if cmd == MINIMUM_OS_VERSION_LOAD_COMMAND:
            minimum_os_version_struct = get_bytes(offset, 24)
            commandtype, cmd_size, platform_type, min_os_ver, sdk_ver, \
                num_tools = unpack("<IIIIII", minimum_os_version_struct)

            return platform_type

        offset += cmdsize

    return -1

def get_platform() -> BROMA_PLATFORMS:
    """Gets the binary's platform
    Returns:
        BROMA_PLATFORMS
    """
    global g_platform
    if g_platform is not None:
        return g_platform

    inf_structure = get_inf_structure()
    file_type = inf_structure.filetype

    if file_type == f_PE:
        g_platform = "win"
    elif file_type == f_MACHO:
        cpu_type = get_dword(
            get_segm_by_sel(selector_by_name("HEADER")).start_ea + 4
        )

        if cpu_type == CPU_TYPE_ARM64:
            platform_type = _get_minimum_mach_o_os_version()

            if platform_type == PLATFORM_TYPE_IOS:
                g_platform = "ios"
            elif platform_type == PLATFORM_TYPE_MACOS:
                g_platform = "m1"
            else:
                # appletv gd real
                ...
        elif cpu_type == CPU_TYPE_X86_64:
            g_platform = "imac"
    elif file_type == f_ELF:
        bitness = get_first_seg().bitness

        if bitness == 0x1:
            g_platform = "android32"
        elif bitness == 0x2:
            g_platform = "android64"
        elif bitness == 0x0:
            # android 16bit real :troll:
            ...

    return g_platform


def get_platform_printable(platform: BROMA_PLATFORMS) -> str:
    """Printable platform name

    Args:
        platform (BROMA_PLATFORMS): The platform

    Returns:
        str: Printable name of said platform
    """
    platform_to_printable = {
        "win": "Windows",
        "imac": "Intel MacOS",  # MacchewOS my beloved
        "m1": "M1 MacOS",
        "ios": "iOS",
        "android32": "Android (32 bit)",
        "android64": "Android (64 bit)"
    }

    return platform_to_printable[platform]


def get_ida_plugin_path() -> Path:
    """Gets the plugin path of the IDA folder, no loger using magic :(

    Returns:
        Path: The plugin path as a pathlib.Path
    """
    return Path(idadir("plugins"))
