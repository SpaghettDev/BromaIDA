VERSION = "5.4.0"
__AUTHOR__ = "SpaghettDev"

PLUGIN_NAME = "BromaIDA"
PLUGIN_HOTKEY = "Ctrl+Shift+B"


from idaapi import (
    msg as ida_msg, register_action, unregister_action,
    plugin_t as ida_plugin_t, action_desc_t as ida_action_desc_t,
    PLUGIN_PROC, PLUGIN_HIDE, PLUGIN_KEEP
)
from ida_kernwin import ask_file, ASKBTN_BTN1, ASKBTN_BTN2
from idautils import Names

from broma_ida.utils import (
    popup, stop, get_platform, get_platform_printable
)
from broma_ida.broma.importer import BromaImporter
from broma_ida.broma.exporter import BromaExporter
from broma_ida.ida_ctx_entry import IDACtxEntry


def bida_main():
    """BromaIDA main entrypoint"""
    import_export_prompt = popup(
        "Import", "Export", "",
        "Import or Export Broma file?\n"
        "(Please make sure Extras.bro is in the same directory)"
    )

    if import_export_prompt == ASKBTN_BTN1:
        filePath = ask_file(False, "GeometryDash.bro", "bro")

        if filePath is None or (filePath and not filePath.endswith(".bro")):
            popup("Ok", "Ok", None, "Please select a valid file!")
            stop()

        platform = get_platform()
        broma_importer = BromaImporter(platform)

        try:
            with open(filePath, "r") as f:
                broma_importer.parse_file_stream(f)
        except FileNotFoundError:
            popup("Ok", "Ok", None, "File doesn't exist? Please try again.")
            stop()

        broma_importer.import_into_idb()

        print("[+] BromaIDA: Finished importing bindings from Broma file")
        popup(
            "Ok", "Ok", None,
            "Finished importing "
            f"{get_platform_printable(platform)} "
            "bindings from Broma file."
        )

    elif import_export_prompt == ASKBTN_BTN2:
        platform = get_platform()

        if platform.startswith("android"):
            popup(
                "Ok", "Ok", None,
                "Cannot export bindings from Android binary!"
            )
            stop()

        # for_saving is not True because we need to read the file first
        # which may not even exist if the saving prompt is used
        # (since you can select files that don't exist within said prompt)
        filePath = ask_file(False, "GeometryDash.bro", "bro")

        if filePath is None or (filePath and not filePath.endswith(".bro")):
            popup("Ok", "Ok", None, "Please select a valid file!")
            stop()

        broma_exporter = BromaExporter(platform, filePath)

        broma_exporter.import_from_idb(Names())
        broma_exporter.export()

        print(
            "[+] BromaIDA: Finished exporting "
            f"{broma_exporter.num_exports} bindings."
        )
        popup("Ok", "Ok", None, "Finished exporting bindings to Broma file.")


class BromaIDAPlugin(ida_plugin_t):
    """BromaIDA Plugin"""
    flags = PLUGIN_PROC | PLUGIN_HIDE
    comment = "Broma support for IDA."
    help = f"{PLUGIN_HOTKEY} to start the importing/exporting."
    wanted_name = PLUGIN_NAME
    wanted_hotkey = PLUGIN_HOTKEY

    ACTION_BTIDA = "btida:run_btida"
    ACTION_DESC = "Launches the BromaIDA plugin."

    def init(self):
        """Ran on plugin load"""
        self._register_action()
        ida_msg(f"{self.wanted_name} v{VERSION} initialized\n")

        return PLUGIN_KEEP

    def term(self):
        """Ran on plugin unload"""
        self._unregister_action()

        ida_msg(f"{self.wanted_name} v{VERSION} unloaded\n")

    def run(self, arg):
        """Ran on "File -> Script File" (shocker) (broken for me :D)"""
        bida_main()

    def _register_action(self):
        """Registers BromaIDA's hotkey"""
        hotkey = ida_action_desc_t(
            self.ACTION_BTIDA,
            "BromaIDA",
            IDACtxEntry(bida_main),
            PLUGIN_HOTKEY,
            self.ACTION_DESC
        )

        register_action(hotkey)

    def _unregister_action(self):
        """Unregisters BromaIDA's hotkey"""
        unregister_action(self.ACTION_BTIDA)


def PLUGIN_ENTRY():
    return BromaIDAPlugin()
