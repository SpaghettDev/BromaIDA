from typing import overload, TypedDict, Literal, NotRequired

from ida_name import is_visible_cp

from broma_ida.broma.argtype import ArgType, RetType


class BindingType(TypedDict):
    """Binding type"""
    name: str
    class_name: str
    qualified_name: NotRequired[str]
    ida_qualified_name: NotRequired[str]
    address: int

    return_type: NotRequired[RetType]
    parameters: NotRequired[list[ArgType]]
    is_virtual: NotRequired[bool]
    is_static: NotRequired[bool]


class Binding:
    """Actual binding type. Implements __eq__ because
    TypedDict can't be instantiated, and as such, can't
    have overridden methods
    """
    binding: BindingType

    def _get_ida_qualified_name(self, binding: BindingType) -> str:
        """Gets the IDA qualified name of a binding.
        (because "~" is not allowed in a method's name,
        unless you modify NameChars in ida.cfg)

        Args:
            binding (BaseShortBindingType | BaseShortBindingTypeWithMD):
                The binding

        Returns:
            str
        """
        if not is_visible_cp(ord("~")) and \
                binding["class_name"] == binding["name"][1:]:
            return f"""{binding["class_name"]}::d{binding["class_name"]}"""
        return f"""{binding["class_name"]}::{binding["name"]}"""

    def __init__(self, binding: BindingType):
        self.binding = BindingType({
            "name": binding["name"],
            "class_name": binding["class_name"],
            "qualified_name":
                f"""{binding["class_name"]}::{binding["name"]}""",
            "ida_qualified_name": self._get_ida_qualified_name(binding),
            "address": binding["address"],
            "return_type": binding.get("return_type") or RetType({
                "name": "", "type": "", "reg": ""
            }),
            "parameters": binding.get("parameters") or [],
            "is_virtual": binding.get("is_virtual") or False,
            "is_static": binding.get("is_static") or False
        })

    @property
    def short_info(self) -> str:
        """Short info about the binding

        Returns:
            str: "[binding qualified name] @ [binding address]"
        """
        return f"""{
            self.binding["qualified_name"]
        } @ {
            hex(self.binding["address"])
        }"""

    def __eq__(self, key: object) -> bool:
        if isinstance(key, int):
            return self.binding["address"] == key
        elif isinstance(key, str):
            return self.binding["qualified_name"] == key

        return False

    @overload
    def __getitem__(
        self,
        key: Literal[
            "name", "class_name", "qualified_name",
            "ida_qualified_name"
        ]
    ) -> str:
        ...

    @overload
    def __getitem__(self, key: Literal["address"]) -> int:
        ...

    @overload
    def __getitem__(
        self,
        key: Literal["is_virtual", "is_static"]
    ) -> bool:
        ...

    @overload
    def __getitem__(self, key: Literal["parameters"]) -> list[ArgType]:
        ...

    @overload
    def __getitem__(self, key: Literal["return_type"]) -> RetType:
        ...

    def __getitem__(self, key):
        if key == "ida_qualified_name":
            self.__update_ida_qualified_name()
        return self.binding.__getitem__(key)

    def __setitem__(self, key, value):
        self.binding[key] = value

    def __str__(self) -> str:
        return f"""{"virtual " if self.binding["is_virtual"] else ""}""" \
            f"""{"static " if self.binding["is_static"] else ""}""" \
            f"""{self.binding["return_type"]["type"]} """ \
            f"""{self.binding["class_name"]}::{self.binding["name"]}""" \
            f"""({", ".join([
                str(arg) for arg in self.binding["parameters"]
            ])}) @ {hex(self.binding["address"])}; """ \
            f"""({self.binding["ida_qualified_name"]})"""

    def __hash__(self) -> int:
        return hash((
            self.binding["name"], self.binding["class_name"],
            self.binding["qualified_name"], self.binding["ida_qualified_name"],
            self.binding["address"]
        ))

    def __update_ida_qualified_name(self):
        self.binding["ida_qualified_name"] = \
            self._get_ida_qualified_name(self.binding)
