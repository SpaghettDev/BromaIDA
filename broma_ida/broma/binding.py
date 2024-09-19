from dataclasses import dataclass, field, is_dataclass

from functools import cache, cached_property

from re import sub

from ida_name import is_visible_cp

from pybroma import FunctionBindField

from broma_ida.broma.argtype import ArgType, RetType
from broma_ida.utils import HAS_IDACLANG, get_platform


@dataclass
class Binding:
    """Binding type"""
    name: str
    class_name: str
    address: int

    ret: RetType = field(default_factory=lambda: RetType(""))
    parameters: list[ArgType] = field(default_factory=list)
    is_virtual: bool = False
    is_static: bool = False

    @staticmethod
    def from_pybroma(class_name: str, f: FunctionBindField):
        func = f.prototype

        return Binding(
            func.name, class_name,
            int(f.binds.platforms_as_dict().get(get_platform(), "-0x1"), 16),
            RetType(func.ret.name.replace("gd::", "std::")),
            [
                ArgType(arg_t.name.replace("gd::", "std::"), name)
                for name, arg_t in func.args.items()
            ],
            func.is_virtual, func.is_static
        )

    @property
    def qualified_name(self) -> str:
        """The qualified name of a binding.

        Returns:
            str: ClassName::MethodName
        """
        return f"{self.class_name}::{self.name}"

    @cached_property
    def ida_qualified_name(self) -> str:
        """The IDA qualified name of a binding.
        (sometimes different from qualified_name because "~"
        isn't a valid symbol for names)

        Returns:
            str
        """
        return f"{self.class_name}::{self.name}".replace(
            "~", "~" if is_visible_cp(ord("~")) else "d"
        )

    @staticmethod
    def __west_rp_ify(t: str) -> str:
        """convert east (ew) reference or pointer (and const) to west one
        (just as god intended)

        Args:
            t (str):

        Returns:
            str
        """
        return sub(
            r"(?:(.*)\s+(&|\*)\s*const|(.*)(?:\s+)const(?:\s*)(&|\*)?)$",
            r"const \1\2\3\4", t
        )

    @cache
    def get_args_str(
            self,
            include_this_arg: bool = True,
            convert_east_to_west_ptr: bool = False
    ) -> str:
        """Gets a function's argument string.

        Args:
            include_this_arg (bool, optional): Include this argument.
                Defaults to True.
            convert_east_to_west_ptr (bool, optional): Converts east pointer,
                reference and const to west one (as god intended).
                Defaults to True.

        Returns:
            str
        """
        args = self.parameters

        if not include_this_arg and self.is_static:
            args = args[1:]

        has_this_arg = False
        if len(args) > 0 and \
                args[0].type == f"{self.class_name}*":
            has_this_arg = True

        if include_this_arg and not self.is_static and \
                not has_this_arg:
            args.insert(0, ArgType(f"{self.class_name}*", "this"))

        return ", ".join([
            self.__west_rp_ify(str(arg)) if convert_east_to_west_ptr
            else str(arg)
            for arg in args
        ])

    def has_same_args(self, other_args: list[ArgType]) -> bool:
        """Compares the binding's arguments to that of another one.
        Used to check if a binding is an overload.

        Args:
            other_args (list[ArgType]): The other binding's arguments.

        Returns:
            bool
        """
        if len(self.parameters) != len(other_args):
            return False

        # assume the user is a good boy and won't export something wrong :D
        if not HAS_IDACLANG:
            return True

        for broma, other in zip(self.parameters, other_args):
            if self.__west_rp_ify(broma.type.replace("*", "&")) != \
                    self.__west_rp_ify(other.type).replace(" *", "*"):
                return False

        return True

    @property
    def short_info(self) -> str:
        """Short info about the binding

        Returns:
            str: "[binding qualified name] @ [binding address]"
        """
        return f"""{self.qualified_name} @ {hex(self.address)}"""

    @cached_property
    def signature(self) -> str:
        """Returns a C++ function signature for the given function.
        Example:
        "virtual void GameObject::setOrientedRectDirty(GameObject*, bool);"

        Returns:
            str
        """
        return \
            f"{'static ' if self.is_static else ''}" \
            f"{'virtual ' if self.is_virtual else ''}" \
            f"{self.ret.type} {self.ida_qualified_name}" \
            f"({self.get_args_str(True, True)});"

    def __eq__(self, value: object) -> bool:
        if isinstance(value, int):
            return self.address == value
        elif isinstance(value, str):
            return self.qualified_name == value
        elif is_dataclass(value):
            return self.__dataclass_fields__ == value.__dataclass_fields__

        return False

    def __hash__(self) -> int:
        return hash((
            self.qualified_name, self.address,
            (str(x) for x in self.parameters)
        ))

    def __str__(self) -> str:
        return f"{'virtual ' if self.is_virtual else ''}" \
            f"{'static ' if self.is_static else ''}" \
            f"{self.ret.type} " \
            f"{self.class_name}::{self.name}" \
            f"""({", ".join([
                str(arg) for arg in self.parameters
            ])}) @ {hex(self.address)}; """ \
            f"({self.ida_qualified_name})"
