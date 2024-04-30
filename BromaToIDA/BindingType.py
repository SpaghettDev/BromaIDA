from typing import Union, TypedDict, Literal, cast


# legacy type
class BaseBindingType(TypedDict):
    """Base binding type"""
    name: str
    className: str
    qualifiedName: str
    idaQualifiedName: str
    inheritedClasses: list[str]
    address: int


class BaseShortBindingType(TypedDict):
    """Base binding type 2"""
    name: str
    className: str
    inheritedClasses: list[str]
    address: int


class Binding:
    """Actual binding type. Implements __eq__ because
    TypedDict can't be instantiated, and as such, can't
    have overloaded methods"""
    binding: BaseBindingType

    def __init__(
        self,
        binding: Union[BaseShortBindingType, BaseBindingType]
    ) -> None:
        if binding.get("qualifiedName"):
            self.binding = cast(BaseBindingType, binding)
        else:
            binding = cast(BaseShortBindingType, binding)

            self.binding = BaseBindingType({
                "name": binding["name"],
                "className": binding["className"],
                "qualifiedName":
                    f"""{binding["className"]}::{binding["name"]}""",
                "idaQualifiedName":
                    f"""{binding["className"]}::{binding["name"]}""".replace(
                        "::~", "::d"
                    ),
                "inheritedClasses": binding["inheritedClasses"],
                "address": binding["address"]
            })

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
            "idaQualifiedName", "inheritedClasses", "address"
        ]
    ) -> Union[str, list[str], int]:
        return self.binding.__getitem__(key)  # type: ignore

    def __str__(self) -> str:
        return f"""name="{self.binding["name"]}" """ \
            f"""className="{self.binding["className"]}" """ \
            f"""qualifiedName="{self.binding["qualifiedName"]}" """ \
            f"""idaQualifiedName="{self.binding["idaQualifiedName"]}" """ \
            f"""inheritedClasses="{self.binding["inheritedClasses"]}" """ \
            f"""address="{self.binding["address"]}\""""

from typing import Union, TypedDict, Literal, cast


# legacy type
class BaseBindingType(TypedDict):
    """Base binding type"""
    name: str
    className: str
    qualifiedName: str
    idaQualifiedName: str
    inheritedClasses: list[str]
    address: int


class BaseShortBindingType(TypedDict):
    """Base binding type 2"""
    name: str
    className: str
    inheritedClasses: list[str]
    address: int


class Binding:
    """Actual binding type. Implements __eq__ because
    TypedDict can't be instantiated, and as such, can't
    have overloaded methods"""
    binding: BaseBindingType

    def __init__(
        self,
        binding: Union[BaseShortBindingType, BaseBindingType]
    ) -> None:
        if binding.get("qualifiedName"):
            self.binding = cast(BaseBindingType, binding)
        else:
            binding = cast(BaseShortBindingType, binding)

            self.binding = BaseBindingType({
                "name": binding["name"],
                "className": binding["className"],
                "qualifiedName":
                    f"""{binding["className"]}::{binding["name"]}""",
                "idaQualifiedName":
                    f"""{binding["className"]}::{binding["name"]}""".replace(
                        "::~", "::d"
                    ),
                "inheritedClasses": binding["inheritedClasses"],
                "address": binding["address"]
            })

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
            "idaQualifiedName", "inheritedClasses", "address"
        ]
    ) -> Union[str, list[str], int]:
        return self.binding.__getitem__(key)  # type: ignore

    def __str__(self) -> str:
        return f"""name="{self.binding["name"]}" """ \
            f"""className="{self.binding["className"]}" """ \
            f"""qualifiedName="{self.binding["qualifiedName"]}" """ \
            f"""idaQualifiedName="{self.binding["idaQualifiedName"]}" """ \
            f"""inheritedClasses="{self.binding["inheritedClasses"]}" """ \
            f"""address="{self.binding["address"]}\""""
