from ida_kernwin import warning as ida_warning, Form

from broma_ida.ui.types.dynamic_form import DynamicForm

from broma_ida.utils import path_exists


class DirectoryInputForm(DynamicForm):
    """
    Simple form that asks for a directory since ida_kernwin
    and its infinite wisdom doesnt have an ask_directory method :D
    Directory string is saved in DirectoryInputForm.saved_controls.iDir
    """

    def __init__(self, input_str: str):
        super().__init__(f"""STARTITEM 0
BUTTON YES NONE
BUTTON NO NONE
BUTTON CANCEL NONE
BromaIDA
{{FormChangeCb}}<{input_str}:{{iDir}}>""", {
            "FormChangeCb": Form.FormChangeCb(self.onFormChange),
            "iDir": Form.DirInput(swidth=35)
        })

    def onFormChange(self, fid: int) -> int:
        super().onFormChange(fid)

        if fid in [-2, -3]:
            dir_value = self.GetControlValue(self.iDir)

            if not path_exists(dir_value):
                ida_warning("Please input a valid path!")
                return 0

        return 1
