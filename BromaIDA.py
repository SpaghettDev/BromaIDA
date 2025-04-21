# flake8-in-file-ignores: noqa: E402
VERSION = "7.0.3"
__AUTHOR__ = "SpaghettDev"

PLUGIN_NAME = "BromaIDA"
PLUGIN_HOTKEY = "Ctrl+Shift+B"


from pathlib import Path

from idaapi import (
    msg as ida_msg, register_action, unregister_action,
    plugin_t as ida_plugin_t, action_desc_t as ida_action_desc_t,
    PLUGIN_PROC, PLUGIN_KEEP
)
from ida_kernwin import ask_file
from idautils import Names

from broma_ida.utils import stop, path_exists, IDAUtils
from broma_ida.broma.importer import BromaImporter
from broma_ida.broma.exporter import BromaExporter
from broma_ida.ida_ctx_entry import IDACtxEntry

from broma_ida.data.data_manager import DataManager

from broma_ida.ui.simple_popup import SimplePopup
from broma_ida.ui.main_form import MainForm


def on_import(form: MainForm, code: int = 0):
    form.Close(1)

    file_path: str = ask_file(False, "GeometryDash.bro", "bro")

    if not path_exists(file_path, ".bro"):
        SimplePopup("Please select a valid file!", "Cancel").show()
        stop()

    broma_importer = BromaImporter(IDAUtils.get_platform(), file_path)
    broma_importer.parse_file()
    broma_importer.import_into_idb()

    print("[+] BromaIDA: Finished importing bindings from Broma file")
    SimplePopup(
        "Finished importing "
        f"{IDAUtils.get_platform_printable()} "
        "bindings from Broma file.",
        "OK"
    ).show()

    DataManager().close()


def on_export(form: MainForm, code: int = 0):
    form.Close(1)

    platform = IDAUtils.get_platform()

    if platform.startswith("android"):
        SimplePopup(
            "Cannot export bindings from Android binary!", "Cancel"
        ).show()
        stop()

    # for_saving is not True because we need to read the file first
    # which may not even exist if the saving prompt is used
    # (since you can select files that don't exist within said prompt)
    file_path: str = ask_file(False, "GeometryDash.bro", "bro")

    if not path_exists(file_path, ".bro"):
        SimplePopup("Please select a valid file!", "Cancel").show()
        stop()

    broma_exporter = BromaExporter(platform, file_path)

    broma_exporter.import_from_idb(Names())
    broma_exporter.export()

    print(
        "[+] BromaIDA: Finished exporting "
        f"{broma_exporter.num_exports} bindings, "
        f"{broma_exporter.num_ret_exports} return types and "
        f"{broma_exporter.num_args_names_exports} argument names."
    )
    SimplePopup("Finished exporting bindings to Broma file.", "OK").show()

    DataManager().close()


def bida_main():
    """BromaIDA main entrypoint"""
    DataManager().init(
        Path.home() / "broma_ida" / "shelf"
    )

    form_code = MainForm(
        VERSION,
        IDAUtils.get_platform_printable(),
        on_import,
        on_export
    ).show()

    # cancel
    if form_code == 0:
        DataManager().close()


class BromaIDAPlugin(ida_plugin_t):
    """BromaIDA Plugin"""
    flags = PLUGIN_PROC
    comment = "Broma support for IDA."
    help = f"{PLUGIN_HOTKEY} to start the importing/exporting."
    wanted_name = PLUGIN_NAME

    ACTION_BTIDA = "bida:run_bida"
    ACTION_DESC = "Launches the BromaIDA plugin."

    def init(self):
        """Ran on plugin load"""
        self._register_action()
        ida_msg(f"{PLUGIN_NAME} v{VERSION} initialized\n")

        return PLUGIN_KEEP

    def term(self):
        """Ran on plugin unload"""
        self._unregister_action()

        ida_msg(f"{PLUGIN_NAME} v{VERSION} unloaded\n")

    def run(self, arg):
        """Ran on "File -> Script File" (shocker)
        (does not work because this plugin has multiple py files)"""
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
