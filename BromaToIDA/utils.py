from typing import NoReturn, Union, Callable

from idaapi import get_imagebase, BADADDR, SN_NOWARN
from ida_kernwin import ask_buttons, del_hotkey, add_hotkey, ASKBTN_BTN1
from ida_name import get_name_ea
from idc import set_name

from BindingType import Binding


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
            print("Hotkey unregistered!")
            del hotkey_ctx  # type: ignore
        else:
            print("Failed to delete hotkey!")
    except:
        hotkey_ctx = add_hotkey(hotkey, f)
        if hotkey_ctx is None:
            print("Failed to register hotkey!")
            del hotkey_ctx
        else:
            print("Hotkey registered!")
