from typing import Callable, Self

from ida_kernwin import Form

from broma_ida.ui.types.dynamic_form import DynamicForm
from broma_ida.ui.settings_form import SettingsForm


class MainForm(DynamicForm):
    """Main form"""

    version: str
    on_import_button: Callable[[Self, int], None]
    on_export_button: Callable[[Self, int], None]

    def __init__(
        self,
        version: str,
        platform: str,
        on_import_button: Callable[[Self, int], None],
        on_export_button: Callable[[Self, int], None]
    ):
        self.version = version
        self.on_import_button = on_import_button
        self.on_export_button = on_export_button

        super().__init__("""STARTITEM 0
BUTTON YES NONE
BUTTON NO NONE
BUTTON CANCEL Cancel
BromaIDA

{cMainLabel}
{cPlatformLabel}

Select Mode:<##Import:{iImportButton}><##Export:{iExportButton}>

    <Settings:{iSettingsButton}>
""", {
            "cMainLabel": Form.StringLabel(
                f"BromaIDA v{version}. IDA Broma Support."
            ),
            "cPlatformLabel": Form.StringLabel(
                f"Current Platform: {platform}"
            ),
            "iImportButton": Form.ButtonInput(self.onImportButton),
            "iExportButton": Form.ButtonInput(self.onExportButton),
            "iSettingsButton": Form.ButtonInput(
                self.onSettingsButton, swidth="27"
            )
        })

    def onImportButton(self, code: int = 0):
        self.on_import_button(self, code)

    def onExportButton(self, code: int = 0):
        self.on_export_button(self, code)

    def onSettingsButton(self, code: int = 0):
        SettingsForm(self.version).show()
