from ida_kernwin import Form

from broma_ida.ui.types.dynamic_form import DynamicForm
from broma_ida.data.data_manager import DataManager

HAS_IDACLANG = False

try:
    import ida_srclang
    del ida_srclang
    HAS_IDACLANG = True
except ImportError:
    pass


class SettingsForm(DynamicForm):
    """The settings form"""

    def __init__(self, version: str):
        super().__init__("""STARTITEM 0
BUTTON YES NONE
BUTTON NO NONE
BUTTON CANCEL NONE
BromaIDA
{FormChangeCb}
<##General Settings#Do not prompt when there is a mismatch in merge information stored in function comments#Always overwrite function comments with merge information:{rAlwaysOverwriteMergeInformation}>
<#Do not prompt when there is a mismatch between IDB and Broma.\nInstead overwrite IDB silently#Always overwrite IDB:{rAlwaysOverwriteIDB}>
<#Skips the Broma hash check that determines if types should be imported.\nBy default, the input Broma file's hash is saved and compared to following imports.\nIf they are the same then BromaIDA will skip importing types.#Disable Broma hash check:{rDisableBromaHashCheck}>
<#Should return types be exported.\nIf enabled, only types that aren't TodoReturn are exported\nBecause of this, it is recommended to only enable this if you have already imported types.#Export return types:{rExportReturnTypes}>
<#Should function arguments' names be exported.\nIf enabled, only argument names that don't match "a[0-9]+" and "p[0-9]+" are exported.#Export function arguments' names:{rExportFunctionArgumentsNames}>{cGeneralSettingsGroup}>

<##                                                                 Import Types Settings#Enables importing of GD and cocos2d classes when importing a Broma file#Import Types:{rImportTypes}>
<#Sets idaclang's parser arguments to suit the current binary's platform#Set Default Parser Parameters:{rSetDefaultParserArguments}>
<#Enable this ONLY if you have genuine Android GNU STL headers#Use Custom Android GNU STL:{rUseCustomAndroidGNUSTL}>
<#Enable this ONLY if you have genuine Mac GNU STL headers#Use Custom Mac GNU STL:{rUseCustomMacGNUSTL}>{cImportTypesSettingsGroup}>
<#Select MSVC STL Directory. Used for Windows binary.\nIf you dont have custom GNU STL set and the binary's platform is not Windows,\nBromaIDA will use this instead with platform specfic modifications#MSVC STL Directory       :{iMSVCSTLDir}>
<#Select Android GNU STL Directory#Android GNU STL Directory:{iAndroidGNUSTLDir}>
<#Select Mac GNU STL Directory#Mac GNU STL Directory    :{iMacGNUSTLDir}>

           {cFooterLabel}
""", {  # noqa: 501
            "FormChangeCb": Form.FormChangeCb(self.onFormChange),
            "cGeneralSettingsGroup": Form.ChkGroupControl((
                "rAlwaysOverwriteMergeInformation", "rDisableBromaHashCheck",
                "rAlwaysOverwriteIDB", "rExportReturnTypes",
                "rExportFunctionArgumentsNames"
            )),
            "cImportTypesSettingsGroup": Form.ChkGroupControl((
                "rImportTypes", "rSetDefaultParserArguments",
                "rUseCustomAndroidGNUSTL", "rUseCustomMacGNUSTL"
            )),
            "iMSVCSTLDir": Form.DirInput(swidth=25),
            "iAndroidGNUSTLDir": Form.DirInput(swidth=25),
            "iMacGNUSTLDir": Form.DirInput(swidth=25),
            "cFooterLabel": Form.StringLabel(
                f"BromaIDA v{version}. Made with love by SpaghettDev"
            )
        })

    def setup(self):
        self.rAlwaysOverwriteMergeInformation.checked = DataManager().get(
            "always_overwrite_merge_information"
        )
        self.rDisableBromaHashCheck.checked = DataManager().get(
            "disable_broma_hash_check"
        )
        self.rAlwaysOverwriteIDB.checked = DataManager().get(
            "always_overwrite_idb"
        )
        self.rExportReturnTypes.checked = DataManager().get(
            "export_return_types"
        )
        self.rExportFunctionArgumentsNames.checked = DataManager().get(
            "export_args_names"
        )

        self.rImportTypes.checked = DataManager().get("import_types")
        self.rSetDefaultParserArguments.checked = DataManager().get(
            "set_default_parser_args"
        )
        self.rUseCustomAndroidGNUSTL.checked = DataManager().get(
            "use_custom_android_gnustl"
        )
        self.rUseCustomMacGNUSTL.checked = DataManager().get(
            "use_custom_mac_gnustl"
        )

        self.iMSVCSTLDir.value = DataManager().get("msvcstl_dir")
        self.iAndroidGNUSTLDir.value = DataManager().get("android_gnustl_dir")
        self.iMacGNUSTLDir.value = DataManager().get("mac_gnustl_dir")

    def onFormChange(self, fid: int) -> int:
        super().onFormChange(fid)

        # General Settings
        if fid in [
            self.rAlwaysOverwriteMergeInformation.id,
            self.rDisableBromaHashCheck.id,
            self.rAlwaysOverwriteIDB.id,
            self.rExportReturnTypes.id,
            self.rExportFunctionArgumentsNames.id
        ]:
            if fid == self.rAlwaysOverwriteMergeInformation.id:
                DataManager().set(
                    "always_overwrite_merge_information",
                    bool(self.GetControlValue(
                        self.rAlwaysOverwriteMergeInformation
                    ))
                )
            elif fid == self.rDisableBromaHashCheck.id:
                DataManager().set(
                    "disable_broma_hash_check",
                    bool(self.GetControlValue(self.rDisableBromaHashCheck))
                )
            elif fid == self.rAlwaysOverwriteIDB.id:
                DataManager().set(
                    "always_overwrite_idb",
                    bool(self.GetControlValue(self.rAlwaysOverwriteIDB))
                )
            elif fid == self.rExportReturnTypes.id:
                DataManager().set(
                    "export_return_types",
                    bool(self.GetControlValue(self.rExportReturnTypes))
                )
            elif fid == self.rExportFunctionArgumentsNames.id:
                DataManager().set(
                    "export_args_names",
                    bool(self.GetControlValue(
                        self.rExportFunctionArgumentsNames
                    ))
                )

        # Import Types Settings
        if fid in [
            self.rImportTypes.id, self.rSetDefaultParserArguments.id,
            self.rUseCustomAndroidGNUSTL.id, self.rUseCustomMacGNUSTL.id
        ]:
            if fid == self.rImportTypes.id:
                DataManager().set(
                    "import_types",
                    bool(self.GetControlValue(self.rImportTypes))
                )
            elif fid == self.rSetDefaultParserArguments.id:
                DataManager().set(
                    "set_default_parser_args",
                    bool(self.GetControlValue(self.rSetDefaultParserArguments))
                )
            elif fid == self.rUseCustomAndroidGNUSTL.id:
                DataManager().set(
                    "use_custom_android_gnustl",
                    bool(self.GetControlValue(self.rUseCustomAndroidGNUSTL))
                )
            elif fid == self.rUseCustomMacGNUSTL.id:
                DataManager().set(
                    "use_custom_mac_gnustl",
                    bool(self.GetControlValue(self.rUseCustomMacGNUSTL))
                )

        # -1 is Form initilalization
        if fid in [
            -1,
            self.rUseCustomAndroidGNUSTL.id, self.rUseCustomMacGNUSTL.id,
            self.rImportTypes.id
        ]:
            # can't call EnableField in setup() because
            # p_fa is not set by then :(
            self.EnableField(
                self.iAndroidGNUSTLDir,
                bool(self.GetControlValue(self.rUseCustomAndroidGNUSTL))
            )
            self.EnableField(
                self.iMacGNUSTLDir,
                bool(self.GetControlValue(self.rUseCustomMacGNUSTL))
            )

            if not HAS_IDACLANG:
                self.EnableField(self.rImportTypes, False)
                DataManager().set("import_types", False)

        # Import Types Directory Inputs
        if fid == self.iMSVCSTLDir.id:
            DataManager().set(
                "msvcstl_dir",
                self.GetControlValue(self.iMSVCSTLDir)
            )
        elif fid == self.iAndroidGNUSTLDir.id:
            DataManager().set(
                "android_gnustl_dir",
                self.GetControlValue(self.iAndroidGNUSTLDir)
            )
        elif fid == self.iMacGNUSTLDir.id:
            DataManager().set(
                "mac_gnustl_dir",
                self.GetControlValue(self.iMacGNUSTLDir)
            )

        # -5 is form destruction
        if fid == -5:
            DataManager().sync()

        return 1
