import idaapi


class IDACtxEntry(idaapi.action_handler_t):
    """Credit: cra0's cvutils-getoffset.py"""
    def __init__(self, action_function):
        idaapi.action_handler_t.__init__(self)
        self.action_function = action_function

    def activate(self, ctx):
        self.action_function()
        return 1

    def update(self, ctx):
        return idaapi.AST_ENABLE_ALWAYS
