from typing import Literal
from dataclasses import dataclass as _dataclass

from ida_kernwin import ask_buttons as _ask_buttons

__all__ = ["AskPopup"]


@_dataclass
class AskPopup:
    """A Yes/No popup. Not a Form."""

    text: str
    """The text to put."""
    button1: str = "Yes"
    """Button 1 text. Defaults to "Yes"."""
    button2: str = "No"
    """Button 2 text. Defaults to "No"."""
    title: str = "BromaIDA"
    """Title text. Defaults to "BromaIDA"."""
    icon: Literal["WARNING", "QUESTION", "INFO"] = "QUESTION"
    """The icon. Defaults to "QUESTION"."""
    autohide: Literal["NONE", "DATABASE", "REGISTRY", "SESSION"] = "NONE"
    """The autohide type. Defaults to "NONE"."""
    default: Literal[0, 1] = 1
    """The default option. Defaults to 1 (button1)."""

    def show(self) -> int:
        """Shows the popup.

        Returns:
            int: The selected button.
        """
        return _ask_buttons(
            self.button1, self.button2, None, self.default,
            f"TITLE {self.title}\nICON {self.icon}\nAUTOHIDE {self.autohide}\n"
            f"HIDECANCEL\n{self.text}"
        )
