from .PyBroma import *
# Inspired by Cython's Compiler Tree Visitors...

class BromaTreeVisitor:
    """Used to safely visit different Parts of a broma file
    and is built to allow for easy visiting of all the parts 
    you need in your code"""

    def visit_PlatformNumber(self, node):
        return 

    def visit_FunctionProto(self, node:FunctionProto):
        return 

    def visit_MemberFunctionProto(self, node:MemberFunctionProto):
        return

    def visit_FunctionBindField(self, node:FunctionBindField):
        self.visit_PlatformNumber(node.binds)
        self.visit_MemberFunctionProto(node.prototype)

    def visit_MemberField(self, node:MemberField):
        pass 

    def visit_OutOfLineField(self, node:OutOfLineField):
        self.visit_MemberFunctionProto(node.prototype)

    def visit_PadField(self, node:PadField):
        self.visit_FunctionProto(node)
 
    def visit_Field(self, f:Field):
        if x := f.getAsFunctionBindField():
            self.visit_FunctionBindField(x)
        elif x := f.getAsMemberField():
            self.visit_MemberField(x)
        elif x := f.getAsOutOfLineField():
            self.visit_OutOfLineField(x)
        elif x := f.getAsPadField():
            self.visit_PadField(x)

    def visit_Function(self, node:Function):
        self.visit_PlatformNumber(node.binds)
        self.visit_FunctionProto(node.prototype)

    def visit_Class(self, node:Class):
        for f in node.fields:
            self.visit_Field(f)
    
    def start(self, root:Root):
        self.root = root
        for c in root.classes:
            self.visit_Class(c)
        for f in root.functions:
            self.visit_Function(f)

    def __call__(self, file:str):
        return self.start(Root(file))
