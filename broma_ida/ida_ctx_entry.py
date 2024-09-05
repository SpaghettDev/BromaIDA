from idaapi import action_handler_t as ida_action_handler_t, AST_ENABLE_ALWAYS


class IDACtxEntry(ida_action_handler_t):
    """Credit: cra0's cvutils-getoffset.py"""
    def __init__(self, action_function):
        super().__init__()
        self.action_function = action_function

    def activate(self, ctx):
        self.action_function()
        return 1

    def update(self, ctx):
        return AST_ENABLE_ALWAYS
