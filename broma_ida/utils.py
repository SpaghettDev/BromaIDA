from typing import NoReturn

from idaapi import (
    get_imagebase, get_inf_structure, decompile,
    BADADDR, SN_NOWARN
)
from ida_kernwin import ask_buttons, ASKBTN_BTN1
from ida_name import get_name_ea
from ida_diskio import idadir
from ida_ida import f_PE, f_MACHO, f_ELF
from ida_segment import get_first_seg
from ida_bytes import get_dword, get_bytes
from ida_segment import get_segm_by_sel
from idc import set_name, selector_by_name
from ida_typeinf import (
    func_type_data_t as ida_func_type_data_t,
    tinfo_t as ida_tinfo_t
)
from ida_nalt import get_tinfo

from struct import unpack
from pathlib import Path

from broma_ida.broma.constants import BROMA_PLATFORMS

# Mach-O Load commands
MINIMUM_OS_VERSION_LOAD_COMMAND = 0x32

# Mach-O CPU types
CPU_TYPE_ARM64 = 0x0100000c
CPU_TYPE_X86_64 = 0x01000007

# Mach-O Platform types
PLATFORM_TYPE_MACOS = 0x1
PLATFORM_TYPE_IOS = 0x2

g_platform: BROMA_PLATFORMS = None  # type: ignore[assignment]


def stop(reason: str | None = None) -> NoReturn:
    """Nuh Uh"""
    raise Exception() if reason is None else Exception(reason)


def ask_popup(
    text: str,
    button1: str = "Yes",
    button2: str = "No",
    default: int = 1
) -> int:
    """Quick ask popup

    Args:
        text (str): The text to put.
        button1 (str, optional): Button 1 text. Defaults to "No".
        button2 (str, optional): Button 2 text. Defaults to "Yes".
        default (int, optional): Default option. Defaults to 1 (button1).
    """
    return ask_buttons(button1, button2, None, default, f"HIDECANCEL\n{text}")


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
            if ask_popup(
                f"{name} is already taken at "
                f"{hex(ida_prev_addr - get_imagebase())} while trying to "
                f"rename {hex(addr)}\n"
                "Overwrite or keep current name?\n"
                "(Old location will be renamed to "
                f"sub_{hex(ida_prev_addr)[2:].upper()})",
                "Overwrite", "Keep"
            ) == ASKBTN_BTN1:
                set_name(
                    ida_prev_addr,
                    f"sub_{hex(ida_prev_addr)[2:]}",
                    SN_NOWARN
                )

    return renamed


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


def get_ida_path(path: str) -> Path:
    """Gets a path relative to the IDA root folder

    Returns:
        Path: The path as a pathlib.Path
    """
    return Path(idadir(path))


def path_exists(path: str, ext: str = "") -> bool:
    """Checks if a path exists.

    Args:
        path (str)
        ext (str, optional): Extention of file. Defaults to "".

    Returns:
        bool
    """
    if path == "" or path is None:
        return False

    if ext == "":
        return Path(path).exists()
    else:
        p_path = Path(path)

        return p_path.suffix == ext and p_path.exists()


def get_function_info(
        ida_ea: int,
        force: bool = False
) -> ida_func_type_data_t:
    """Gets the info about a function

    Args:
        ida_ea (int): The function address
        force (bool, optional): Should the data be forcefully gotten.
            Forces a decompilation of ida_ea. Defaults to False.

    Returns:
        ida_func_type_data_t: The func_type_data_t of the function.
        Returns None only if the function is too big to decompile
    """
    tif = ida_tinfo_t()
    func_info = ida_func_type_data_t()

    if get_tinfo(tif, ida_ea):
        if tif.get_func_details(func_info):
            return func_info

    # if we reached here then get_func_details
    # returned False for no fucking reason

    if not force:
        return None

    xfunc = decompile(ida_ea)

    if xfunc is None:
        # function is too big to decompile, or some other decomp error
        return None

    xfunc.type.get_func_details(func_info)

    return func_info
