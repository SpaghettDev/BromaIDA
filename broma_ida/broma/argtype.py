from typing import cast, overload, Union, Optional, TypedDict, Literal

from re import split, match, sub


class BaseArgType(TypedDict):
    """A type. Has a register, a name and an actual type."""
    name: str
    type: str
    reg: Optional[str]


class BaseShortArgType(TypedDict):
    """A type. Has an (optional) register, a name and an actual type."""
    name: str
    type: str


BASE_EXPANDED_STL_TYPES = {
    "std::vector": "<{}, std::allocator<{}>>",
    "std::map":
        "<{0}, {1}, std::less<{0}>, std::allocator<std::pair<const {0}, {1}>>>",  # noqa: E501
    "std::unordered_map":
        "<{0}, {1}, std::hash<{0}>, std::equal_to<{0}>, std::allocator<std::pair<const {0}, {1}>>>",  # noqa: E501
    "std::set": "<{}, std::less<{}>, std::allocator<{}>>",
    "std::unordered_set":
        "<{}, std::hash<{}>, std::equal_to<{}>, std::allocator<{}>>"
}

EXPANDED_STL_TYPES = {
    "std::string":
        "std::basic_string<char, std::char_traits<char>, std::allocator<char>>"
}


class ArgType:
    """An argument type"""
    btype: BaseArgType

    def _expand_stl_type(self, stl_type: str) -> str:
        """Expands STL types because IDA is retarded and likes to expand them

        Args:
            stl_type (str)

        Returns:
            str
        """
        # IDA likes spaces after types
        format_pointer = lambda pt: sub(
            r"([^ ])\*", r"\1 *", pt
        )  # noqa: E731

        if "std::" not in stl_type:
            return stl_type

        if sub(
            "(?: )?const(?: )?", "", stl_type
        ).removesuffix("&").removesuffix("*") == "std::string":
            return stl_type

        if stl_type.startswith("std::vector"):
            split_type = match(r"std::vector<(.*)>", stl_type)

            assert split_type is not None, f"Couldn't get contained type for '{stl_type}'"

            if split_type.group(1) in EXPANDED_STL_TYPES:
                return format_pointer(f"""std::vector{
                    BASE_EXPANDED_STL_TYPES["std::vector"]
                }""".replace(
                    "{}",
                    EXPANDED_STL_TYPES[split_type.group(1)]  # type: ignore
                ))

            # this should never happen, but it causes the below code
            # to go ham
            if "std::allocator" in stl_type:
                return stl_type

            # "it just works"
            # credit to henrysck075 i couldnt figure out this shit :D
            ret = stl_type
            vec_to_contained = [[ret, ret]]
            while True:
                try:
                    ret = match(r"std::vector<(.+)>", ret)

                    if ret is None:
                        break

                    ret = ret.group(1)
                except IndexError:
                    break
                vec_to_contained.append([ret, ret])

            split_type = vec_to_contained[-1][0]
            vec_to_contained[-2][1] = vec_to_contained[-2][1].replace(
                f"<{split_type}>",
                BASE_EXPANDED_STL_TYPES["std::vector"].replace(
                    "{}", split_type
                )
            )

            for i in reversed(range(len(vec_to_contained) - 2)):
                expanded_vec = vec_to_contained[i+1][1]
                vec_to_contained[i][1] = vec_to_contained[i][1].replace(
                    f"<{vec_to_contained[i+1][0]}>",
                    BASE_EXPANDED_STL_TYPES["std::vector"].replace(
                        "{}", expanded_vec
                    )
                )

            return format_pointer(vec_to_contained[0][1])

        if stl_type.startswith("std::map"):
            split_type = split(r"std::map<(.*),(?: )?(.*)>", stl_type)

            assert split_type is not None, f"Couldn't get contained types for '{stl_type}'"

            map_key_type = ""
            map_value_type = ""

            # key or value is an STL type that has 2 template args
            if "," in split_type[1] and split_type[2].endswith(">"):
                map_key_type = split_type[1][:split_type[1].index(" ") - 1]
                map_value_type = f"""{
                    split_type[1][split_type[1].index(" ") + 1:]
                }, {
                    split_type[2]
                }"""
            else:
                map_key_type = split_type[1]
                map_value_type = split_type[2]

            if map_key_type in EXPANDED_STL_TYPES or \
                    map_value_type in EXPANDED_STL_TYPES:
                key_is_stl = map_key_type in EXPANDED_STL_TYPES
                value_is_stl = map_value_type in EXPANDED_STL_TYPES

                return format_pointer(f"""std::map{
                    BASE_EXPANDED_STL_TYPES["std::map"]
                }""".replace(
                    "{0}",
                    EXPANDED_STL_TYPES[map_key_type]
                    if key_is_stl else map_key_type
                ).replace(
                    "{1}",
                    EXPANDED_STL_TYPES[map_value_type]
                    if value_is_stl else map_value_type
                ))

            # std::map is never used as key
            if any([
                x in map_value_type for x in BASE_EXPANDED_STL_TYPES.keys()
            ]):
                return format_pointer(f"""std::map{
                    BASE_EXPANDED_STL_TYPES["std::map"]
                }""".replace(
                    "{0}", map_key_type
                ).replace(
                    "{1}", self._expand_stl_type(map_value_type)
                ))

            return format_pointer(f"""std::map{
                BASE_EXPANDED_STL_TYPES["std::map"]
            }""".replace(
                "{0}", map_key_type
            ).replace(
                "{1}", map_value_type
            ))

        if stl_type.startswith("std::unordered_map"):
            split_type = split(r"std::unordered_map<(.*),(?: )?(.*)>", stl_type)

            assert split_type is not None, f"Couldn't get contained types for {stl_type}"

            map_key_type = ""
            map_value_type = ""

            if "," in split_type[1] and split_type[2].endswith(">"):
                map_key_type = split_type[1][:split_type[1].index(" ") - 1]
                map_value_type = f"""{
                    split_type[1][split_type[1].index(" ") + 1:]
                }, {
                    split_type[2]
                }"""
            else:
                map_key_type = split_type[1]
                map_value_type = split_type[2]

            if map_key_type in EXPANDED_STL_TYPES or \
                    map_value_type in EXPANDED_STL_TYPES:
                key_is_stl = map_key_type in EXPANDED_STL_TYPES
                value_is_stl = map_value_type in EXPANDED_STL_TYPES

                return format_pointer(f"""std::unordered_map{
                    BASE_EXPANDED_STL_TYPES["std::unordered_map"]
                }""".replace(
                    "{0}",
                    EXPANDED_STL_TYPES[map_key_type]
                    if key_is_stl else map_key_type
                ).replace(
                    "{1}",
                    EXPANDED_STL_TYPES[map_value_type]
                    if value_is_stl else map_value_type
                ))

            # std::unordered_map is never used as key
            if any([
                x in map_value_type for x in BASE_EXPANDED_STL_TYPES.keys()
            ]):
                return format_pointer(f"""std::unordered_map{
                    BASE_EXPANDED_STL_TYPES["std::unordered_map"]
                }""".replace(
                    "{0}", map_key_type
                ).replace(
                    "{1}", self._expand_stl_type(map_value_type)
                ))

            return format_pointer(f"""std::unordered_map{
                BASE_EXPANDED_STL_TYPES["std::unordered_map"]
            }""".replace(
                "{0}", map_key_type
            ).replace(
                "{1}", map_value_type
            ))

        if stl_type.startswith("std::set"):
            contained = match("std::set<(.*)>", stl_type)

            assert contained is not None, f"Couldn't get contained type for {stl_type}"

            return format_pointer(f"""std::set{
                BASE_EXPANDED_STL_TYPES["std::set"]
            }""".replace(
                "{}", contained.group(1)
            ))

        if stl_type.startswith("std::unordered_set"):
            contained = match("std::unordered_set<(.*)>", stl_type)

            assert contained is not None, f"Couldn't get contained type for {stl_type}"

            return format_pointer(f"""std::unordered_set{
                BASE_EXPANDED_STL_TYPES["std::unordered_set"]
            }""".replace(
                "{}", contained.group(1)
            ))

        raise BaseException(f"[!] Couldn't expand STL type: '{stl_type}'")

    def __init__(self, btype: Union[BaseArgType, BaseShortArgType]):
        if btype.get("reg"):
            self.btype = cast(BaseArgType, btype)
        else:
            self.btype = {
                "name": btype["name"],
                "type": btype["type"],
                "reg": None
            }

        self.btype["type"] = self._expand_stl_type(self.btype["type"])

    def __str__(self) -> str:
        if self.btype["name"] == "":
            return self.btype["type"]

        return f"""{self.btype["type"]} {self.btype["name"]}{
            f"@<{self.btype['reg']}>" if self.btype["reg"] is not None else ""
        }"""

    def __eq__(self, key: object) -> bool:
        if isinstance(key, str):
            return self.btype["type"] == key

        return False

    @overload
    def __getitem__(
        self,
        key: Literal[
            "name", "type"
        ]
    ) -> str:
        ...

    @overload
    def __getitem__(self, key: Literal["reg"]) -> Optional[str]:
        ...

    def __getitem__(self, key):
        return self.btype.__getitem__(key)

    def __setitem__(self, key, value):
        self.btype[key] = value


class RetType(ArgType):
    """A return type"""
