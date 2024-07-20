from typing import cast, overload, Union, TypedDict, Literal

from ida_name import is_visible_cp

from broma_ida.broma.argtype import ArgType, RetType


class BaseBindingType(TypedDict):
    """Base binding type"""
    name: str
    class_name: str
    qualified_name: str
    ida_qualified_name: str
    address: int

    return_type: RetType
    parameters: list[ArgType]
    is_virtual: bool
    is_static: bool


class BaseShortBindingType(TypedDict):
    """Base binding type (but shorter)"""
    name: str
    class_name: str
    address: int


class BaseShortBindingTypeWithMD(BaseShortBindingType):
    """Base binding type (shorter, with metadata about the function)"""
    return_type: RetType
    parameters: list[ArgType]
    is_virtual: bool
    is_static: bool


class Binding:
    """Actual binding type. Implements __eq__ because
    TypedDict can't be instantiated, and as such, can't
    have overridden methods
    """
    binding: BaseBindingType
    is_overload: bool

    def _get_ida_qualified_name(
        self,
        binding: Union[
            BaseShortBindingType, BaseShortBindingTypeWithMD
        ]
    ) -> str:
        """Get's the IDA qualified name of a binding.
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

    def __init__(
        self,
        binding: Union[
            BaseShortBindingType, BaseShortBindingTypeWithMD, BaseBindingType
        ],
        is_overload: bool = False
    ) -> None:
        if binding.get("qualified_name") is not None:
            self.binding = cast(BaseBindingType, binding)
        else:
            binding = cast(BaseShortBindingType, binding)

            if binding.get("return_type"):
                binding = cast(BaseShortBindingTypeWithMD, binding)

            self.binding = BaseBindingType({
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

        self.is_overload = is_overload

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

    def update(self):
        self.binding["ida_qualified_name"] = \
            self._get_ida_qualified_name(self.binding)
