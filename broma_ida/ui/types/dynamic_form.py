from typing import Any

from ida_kernwin import py_register_compiled_form, Form

__all__ = ["DynamicFormControls", "DynamicForm"]

SKIPPED_CONTROLS = (Form.FormChangeCb, Form.ButtonInput)


class DynamicFormControls:
    """This class's members are added dynamically"""

    def add_control(self, name: str, value: Any):
        self.__setattr__(name, value)


class DynamicForm(Form):
    """
    An ida_kernwin.Form that automatically frees its resources after its closed
    (amazing api ida... what is this? fuckin C??)
    """

    saved_controls: DynamicFormControls
    """
    The form's controls. For use after the Form finishes
    executing and is closed.
    """

    def __init__(self, form, controls):
        self.saved_controls = DynamicFormControls()

        super().__init__(form, controls)

    def __save_controls(self):
        """Saves the controls from Form.__controls."""
        for name, ctrl in self.controls.items():
            if isinstance(ctrl, SKIPPED_CONTROLS):
                continue

            self.save_control(name, ctrl)

    def save_control(self, name: str, control: Form.Control):
        """Saves a control's value.

        Args:
            name (str): The control's name
            control (Form.Control): The control
        """
        try:
            control_value = self.GetControlValue(control)

            if control_value is None:
                raise ValueError

            self.saved_controls.add_control(name, control_value)
        except (NotImplementedError, ValueError):
            value: Any = None

            if isinstance(control, Form.StringLabel):
                value = control.arg.value.decode()
            elif hasattr(control, "value"):
                value = control.value
            elif hasattr(control, "checked"):
                value = bool(control.checked)

            assert value is not None, \
                f"Couldn't save control '{control}' of type '{type(control)}'"

            self.saved_controls.add_control(name, value)

    def onFormChange(self, fid: int) -> int:
        """The form change callback.
        If no FormChangeCb is provided, this one is injected.
        If you override call the original so controls save their actual state.

        Args:
            fid (int): The control's id

        Returns:
            int
        """
        self.__save_controls()

        return 1

    def setup(self):
        """Invoked after the Form is Compiled"""

    def show(self) -> int:
        """Displays the form and then frees its resources after it's closed

        Returns:
            int: return code of the form
        """
        # In case a FormChangeCb isn't provided, we need to inject ours
        # early on in Compile()'s execution, so we recreate it
        # (didn't want to override CompileEx())

        # A slight side effect is onFormChange will always be registered as
        # the Form's change callback, and as a result
        # just overriding onFormChange is enough to register a custom form
        # change callback, even if you didn't provide one in the
        # form string. But I find that a net positive imo

        self._reset()

        self.AddControls(self.controls)

        args = self.CompileEx(self.form)

        if "___dummyfchgcb" in self._Form__controls:
            form_change_cb = Form.FormChangeCb(self.onFormChange)
            form_change_cb.form = self
            form_change_cb.parent_hasattr = True

            self._Form__controls["___dummyfchgcb"].free()
            self._Form__controls["___dummyfchgcb"] = form_change_cb
            args[1 if self.modal else 2] = form_change_cb.get_arg()

        self._Form__args = args

        py_register_compiled_form(self)

        self.setup()
        code = self.Execute()
        self.Free()

        return code
