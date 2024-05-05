from typing import Union, TypedDict, Literal, cast

from pybroma import Type


class BaseBindingType(TypedDict):
    """Base binding type"""
    name: str
    className: str
    qualifiedName: str
    idaQualifiedName: str
    address: int

    return_type: Type
    parameters: dict[str, Type]


class BaseShortBindingType(TypedDict):
    """Base binding type (but shorter)"""
    name: str
    className: str
    address: int


class BaseShortBindingTypeWithMD(BaseShortBindingType):
    """Base binding type (shorter, with metadata about the function)"""
    return_type: Type
    parameters: dict[str, Type]


class Binding:
    """Actual binding type. Implements __eq__ because
    TypedDict can't be instantiated, and as such, can't
    have overridden methods
    TODO: figure out a way to narrow down the return type of __getitem__
    to stop putting type: ignore everywhere
    """
    binding: BaseBindingType
    is_overload: bool

    def __init__(
        self,
        binding: Union[
            BaseShortBindingType, BaseShortBindingTypeWithMD, BaseBindingType
        ],
        is_overload: bool = False
    ) -> None:
        if binding.get("qualifiedName") is not None:
            self.binding = cast(BaseBindingType, binding)
        else:
            binding = cast(BaseShortBindingType, binding)

            if binding.get("return_type"):
                binding = cast(BaseShortBindingTypeWithMD, binding)

            self.binding = BaseBindingType({
                "name": binding["name"],
                "className": binding["className"],
                "qualifiedName":
                    f"""{binding["className"]}::{binding["name"]}""",
                "idaQualifiedName":
                    f"""{binding["className"]}::{binding["name"]}""".replace(
                        "::~", "::d"
                    ),
                "address": binding["address"],
                "return_type": binding.get("return_type") or Type(),
                "parameters": binding.get("parameters") or {}
            })

        self.is_overload = is_overload

    def __eq__(self, key: object) -> bool:
        if isinstance(key, int):
            return self.binding["address"] == key
        elif isinstance(key, str):
            return self.binding["qualifiedName"] == key

        return False

    def __getitem__(
        self,
        key: Literal[
            "name", "className", "qualifiedName",
            "idaQualifiedName", "address", "return_type",
            "parameters"
        ]
    ) -> Union[str, list[str], int, bool]:
        return self.binding.__getitem__(key)  # type: ignore

    def __str__(self) -> str:
        return f"""name={self.binding["name"]}""" \
            f"""className={self.binding["className"]}""" \
            f"""qualifiedName={self.binding["qualifiedName"]}""" \
            f"""idaQualifiedName={self.binding["idaQualifiedName"]}""" \
            f"""address={hex(self.binding["address"])} """ \
            f"""is_overload={self.is_overload} """ \
            f"""{self.binding["return_type"]}({
                ', '.join(self.binding["parameters"])
            })"""

    def __hash__(self) -> int:
        return hash((
            self.binding["name"], self.binding["className"],
            self.binding["qualifiedName"], self.binding["idaQualifiedName"],
            self.binding["address"]
        ))
