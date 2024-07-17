from typing import Literal

BROMA_PLATFORMS = Literal["win", "imac", "m1", "ios", "android32", "android64"]

BROMA_CALLING_CONVENTIONS = Literal[
    "default", "thiscall",
    "optcall", "membercall"
]

# not a Broma constant but a constant nonetheless
CALLING_CONVENTIONS = Literal[
    "__thiscall", "__fastcall",
    "__cdecl", "__stdcall"
]

CPP_TYPE_SPECIFIERS = ("unsigned", "signed")
CPP_TYPE_QUALIFIERS = ("const", "volatile")
CPP_DATA_TYPES = ("bool", "char", "short", "int", "long")
CPP_PRIMITIVES = (
    "void", "int", "char", "float", "short",
    "double", "bool", "long"
)

DATA_TYPE_TO_SIZE = {
    "long": 8,
    "int": 4,
    "short": 2,
    "char": 1,
    "bool": 1
}
