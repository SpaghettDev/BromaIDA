VERSION = "1.0.0"
__AUTHOR__ = "SpaghettDev"

PLUGIN_NAME = "Broma to IDA Importer (real)"

from idaapi import get_imagebase
from ida_kernwin import ask_file, ASKBTN_BTN1, ASKBTN_CANCEL
from ida_funcs import add_func, get_func
from idc import get_name, get_func_flags, get_func_cmt, set_func_cmt, FUNC_LIB

from re import sub

from BromaToIDA.BindingType import Binding
from BromaToIDA.utils import popup, stop, rename_func, get_short_info, register_btida
from BromaToIDA.parser import parse_broma


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
                [binding["qualifiedName"]
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
