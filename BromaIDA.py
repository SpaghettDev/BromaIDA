VERSION = "2.0.0"
__AUTHOR__ = "SpaghettDev"

PLUGIN_NAME = "BromaIDA"
PLUGIN_HOTKEY = "Ctrl+Shift+B"

import idaapi
import ida_kernwin
import ida_funcs
import idc
import idautils

from typing import cast
from re import sub
from io import TextIOWrapper

from broma_ida.binding_type import Binding
from broma_ida.utils import (
    popup, stop, rename_func, get_short_info, get_platform,
    get_platform_printable
)
from broma_ida.broma.parser import BromaParser
from broma_ida.broma.exporter import BromaExporter
from broma_ida.ida_ctx_entry import IDACtxEntry


def bida_main():
    """BromaIDA main entrypoint"""
    import_export_prompt = popup(
        "Import", "Export", "",
        "Import or Export Broma file?"
    )

    if import_export_prompt == ida_kernwin.ASKBTN_BTN1:
        filePath = ida_kernwin.ask_file(False, "GeometryDash.bro", "bro")

        if filePath is None or (filePath and not filePath.endswith(".bro")):
            popup("Ok", None, None, "Please select a valid file!")
            stop()

        platform = get_platform()
        broma_parser = BromaParser(platform)

        try:
            with open(filePath, "r") as f:
                broma_parser.parse_file_stream(cast(TextIOWrapper, f))
        except FileNotFoundError:
            popup("Ok", None, None, "File doesn't exist? Please try again.")
            stop()

        print(
            f"\n\n[+] Read {len(broma_parser.bindings)} "
            f"{get_platform_printable(platform)} bindings from {filePath}"
        )
        print(
            f"[+] Read {len(broma_parser.duplicates)} "
            f"duplicate {get_platform_printable(platform)} "
            f"bindings from {filePath}\n"
        )

        # first, handle non-duplicates
        for binding in broma_parser.bindings:
            ida_ea = idaapi.get_imagebase() + binding["address"]
            ida_name = idc.get_name(ida_ea)
            ida_func_flags = idc.get_func_flags(ida_ea)

            if ida_name.startswith("loc_"):
                ida_funcs.add_func(ida_ea)

            if ida_func_flags & idc.FUNC_LIB:
                print(
                    f"[!] Tried to rename a library function! "
                    f"({get_short_info(binding)})"
                )
                continue

            if ida_funcs.get_func(ida_ea).start_ea != ida_ea:
                print(
                    f"[!] Function is in the middle of another one! "
                    f"({get_short_info(binding)})"
                )
                continue

            if ida_name.startswith("sub_"):
                rename_func(
                    ida_ea,
                    binding["idaQualifiedName"]  # type: ignore
                )
            elif sub("_[0-9]+", "", ida_name) != binding["idaQualifiedName"]:
                mismatch_popup = popup(
                    "Overwrite", "Keep", "",
                    f"""Mismatch in Broma ({binding["qualifiedName"]}) """
                    f"and idb ({ida_name})!\n"
                    "Overwrite from Broma or keep current name?"
                )

                if mismatch_popup == ida_kernwin.ASKBTN_BTN1:
                    rename_func(
                        ida_ea,
                        binding["idaQualifiedName"]  # type: ignore
                    )
                elif mismatch_popup == ida_kernwin.ASKBTN_CANCEL:
                    stop()

        # and now for what took me 3 hours :D
        for addr, bindings in broma_parser.duplicates.items():
            ida_ea = idaapi.get_imagebase() + addr

            func_cmt = idc.get_func_cmt(ida_ea, True)
            func_names = ", ".join(
                [binding["qualifiedName"]
                    for binding in bindings]  # type: ignore
            )

            if func_cmt == "":
                # use the first occurrence as the name (very good imo)
                rename_func(
                    ida_ea,
                    bindings[0]["idaQualifiedName"]  # type: ignore
                )

                idc.set_func_cmt(ida_ea, f"Merged with: {func_names}", True)
            elif func_cmt.startswith("Merged with: "):
                cmt_func_names = func_cmt.lstrip("Merged with: ")

                if func_names != cmt_func_names:
                    print(
                        "[!] Mismatch in merged function list "
                        f"(Current: {cmt_func_names} | Correct: {func_names})! "
                        "Correcting..."
                    )
                    idc.set_func_cmt(
                        ida_ea, f"Merged with: {func_names}", True
                    )
            else:
                if popup(
                    "Overwrite", "Keep", None,
                    f"{hex(addr)} already has a comment! Would you like to "
                    "overwrite it with merge information or keep the current "
                    "comment?\n"
                    "(You will be prompted with this again if you rerun the "
                    "script and there are merged functions!)"
                ) == ida_kernwin.ASKBTN_BTN1:
                    idc.set_func_cmt(
                        ida_ea, f"Merged with: {func_names}", True
                    )

        print("[+] Finished importing bindings from Broma file")
        popup(
            "Ok", "Ok", None,
            f"Finished importing {get_platform_printable(platform)} "
            "bindings from Broma file."
        )
        broma_parser.reset()

    elif import_export_prompt == ida_kernwin.ASKBTN_BTN2:
        platform = get_platform()

        filePath = ida_kernwin.ask_file(True, "GeometryDash.bro", "bro")

        if filePath is None or (filePath and not filePath.endswith(".bro")):
            popup("Ok", None, None, "Please select a valid file!")
            stop()

        broma_exporter = BromaExporter(platform, filePath)

        for ea, name in idautils.Names():
            if "::" not in name:
                continue

            split_name = name.split("::")

            # ["GJUINode", "dGJUINode"] -> "GJUINode" == "GJUINode"
            if split_name[0] == split_name[1][1:]:
                split_name = [
                    split_name[0],
                    split_name[1].replace("::d", "::~")
                ]

            broma_exporter.push_binding(Binding({
                "name": split_name[1],
                "className": split_name[0],
                "inheritedClasses": [],
                "address": ea - idaapi.get_imagebase()
            }))

        broma_exporter.export()

        print(f"[+] Finished exporting {broma_exporter.num_exports} bindings.")
        popup("Ok", "Ok", None, "Finished exporting bindings to Broma file.")
        broma_exporter.reset()


class BromaIDAPlugin(idaapi.plugin_t):
    """BromaIDA Plugin"""
    flags = idaapi.PLUGIN_PROC | idaapi.PLUGIN_HIDE
    comment = "Broma support for IDA."
    help = "Ctrl-Shift-I to start the importing/exporting."
    wanted_name = PLUGIN_NAME
    wanted_hotkey = PLUGIN_HOTKEY

    ACTION_BTIDA = "btida:run_btida"
    ACTION_DESC = "Launches the BromaIDA plugin."

    def init(self):
        """Ran on plugin load"""
        self._register_action()

        idaapi.msg(f"{self.wanted_name} v{VERSION} initialized\n")

        return idaapi.PLUGIN_KEEP

    def term(self):
        """Ran on plugin unload"""
        self._unregister_action()

        idaapi.msg(f"{self.wanted_name} v{VERSION} unloaded\n")

    def run(self, arg):
        """Ran on "File -> Script File" (shocker) (broken for me :D)"""
        bida_main()

    def _register_action(self):
        """Registers BromaIDA's hotkey"""
        hotkey = idaapi.action_desc_t(
            self.ACTION_BTIDA,
            "BromaIDA",
            IDACtxEntry(bida_main),
            PLUGIN_HOTKEY,
            self.ACTION_DESC
        )

        idaapi.register_action(hotkey)

    def _unregister_action(self):
        """Unregisters BromaIDA's hotkey"""
        idaapi.unregister_action(self.ACTION_BTIDA)


def PLUGIN_ENTRY():
    return BromaIDAPlugin()
