from ida_kernwin import Form

from broma_ida.ui.types.dynamic_form import DynamicForm


class SimplePopup(DynamicForm):
    def __init__(
        self,
        content: str,
        button1: str,
        button2: str | None = None,
        button3: str | None = None,
        default: int = 1,
        title: str = "BromaIDA"
    ):
        """Creates a simple popup

        Args:
            content (str): Popup content
            button1 (str): Button 1 text
            button2 (str, optional): Button 2 text. Defaults to None (hidden).
            button3 (str, optional): Button 3 text. Defaults to None (hidden).
            title (str, optional): Title of the popup. Defaults to "BromaIDA".
        """
        set_dflt = lambda x: "*" if x == default else ""  # noqa: E731

        super().__init__(f"""STARTITEM 0
BUTTON YES{set_dflt(1)} {button1}
BUTTON NO{set_dflt(2)} {"NONE" if button2 is None else button2}
BUTTON CANCEL{set_dflt(3)} {"NONE" if button3 is None else button3}
{title}
{{label}}""", {
            "label": Form.StringLabel(content)
        })
