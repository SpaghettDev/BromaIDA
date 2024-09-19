from typing import cast, Final, Union

from dataclasses import dataclass, is_dataclass

from re import sub


class ATUtils:
    """ArgType utilities."""

    STL_TREE = list[str | list[Union[str, "STL_TREE"]]]
    STL_EXPANSION_MAP: Final = {
        "std::map": "std::map<{0}, {1}, std::less<{0}>, std::allocator<std::pair<const {0}, {1}>>>",  # noqa: E501
        "std::unordered_map":
            "std::unordered_map<{0}, {1}, std::hash<{0}>, std::equal_to<{0}>, std::allocator<std::pair<const {0}, {1}>>>",  # noqa: E501
        "std::vector": "std::vector<{0}, std::allocator<{0}>>",
        "std::set": "std::set<{0}, std::less<{0}>, std::allocator<{0}>>",
        "std::unordered_set": "std::unordered_set<{0}, std::hash<{0}>, std::equal_to<{0}>, std::allocator<{0}>>",  # noqa: E501
        "std::list": "std::list<{0}, std::allocator<{0}>>",
        "std::deque": "std::deque<{0}, std::allocator<{0}>>",

        # ditto
        "std::pair": "std::pair<{0}, {1}>",

        # not really a templated type, but whatever
        "std::string": "std::basic_string<char, std::char_traits<char>, std::allocator<char>>"  # noqa: E501
    }
    """Dictionary of STL type to its expanded form."""

    has_two_templates = lambda s: "{1}" in s  # noqa" E731
    """Does the STL type take 2 templates."""

    format_ptr = lambda pt: sub(  # noqa: E731
        r"([^ ])\*", r"\1 *", pt
    )
    """IDA is east pointer (ew)"""

    strip_crp = lambda tt: (  # noqa: E731
        cast(str, tt)
        .removesuffix("&")
        .removesuffix("*")
        .removeprefix("const ")
        .removesuffix(" const")
        .lstrip().rstrip()
    )
    """Strips const, reference and pointer from the type."""

    @staticmethod
    def split_stl_type(stl_t: str) -> STL_TREE:
        """Splits an STL type string into a list of STL type name
        and contained types.

        Args:
            stl_t (str): STL type string.

        Returns:
            list[str | list[str]]: ["", "std::type", ["const", "T1", ""], ["", "T2, "*"], "&"]
        """  # noqa: E501
        ptr = stl_t[-1] if stl_t[-1] in ("*", "&") else ""
        const = "const" if stl_t.startswith("const") or \
            stl_t.removesuffix(ptr).rstrip().endswith("const") else ""

        if "std::" not in stl_t or ATUtils.strip_crp(stl_t) == "std::string":
            return [const, ATUtils.strip_crp(stl_t), ptr]

        split_stl_t: ATUtils.STL_TREE = [const]
        nest_count = 0
        current_token: str = ""

        stl_t = stl_t.replace(const, "")
        split_stl_t.append(stl_t[:stl_t.index("<")].lstrip())
        stl_t = stl_t[stl_t.index("<")+1:stl_t.rindex(">")]

        for i, c in enumerate(stl_t):
            if c == "<":
                nest_count += 1
            elif c == ">":
                nest_count -= 1

            if nest_count == 0 and (c == "," or i == len(stl_t) - 1):
                if c != ",":
                    current_token += c

                split_stl_t.append(
                    ATUtils.split_stl_type(current_token.strip())
                )
                if c != ",":
                    split_stl_t.append(ptr)

                current_token = ""
            else:
                current_token += c

        return split_stl_t

    @staticmethod
    def expand_stl_type(stl_t: str) -> str:
        """Expands an STL type.
        Example:
        "std::map<int, int>"
        into
        "std::map<int, int, std::less<int>, std::allocator<std::pair<const int, int>>>"

        Args:
            stl_t (str): The unexpanded STL type

        Returns:
            str
        """  # noqa: E501
        def flatten_and_expand(s: ATUtils.STL_TREE) -> str:
            r: list[str] = []

            for i, t in enumerate(s):
                if isinstance(t, str):
                    if "std::" not in t:
                        r.append(t)
                        continue

                    if t == "std::string":
                        return "{} {}{}".format(
                            cast(str, s[0]),
                            ATUtils.STL_EXPANSION_MAP["std::string"],
                            cast(str, s.pop(-1))
                        ).lstrip()

                    expanded_stl_t = ATUtils.STL_EXPANSION_MAP[t]

                    r.append(expanded_stl_t)

                    if "std::" in s[i + 1][1]:
                        r.append(flatten_and_expand(cast(list, s.pop(i + 1))))
                    else:
                        contained = cast(list[str], s.pop(i + 1))
                        r.append("{} {}{}".format(*contained).lstrip())

                    if ATUtils.has_two_templates(expanded_stl_t):
                        if "std::" in s[i + 1][1]:
                            r.append(
                                flatten_and_expand(cast(list, s.pop(i + 1)))
                            )
                        else:
                            contained = cast(list[str], s.pop(i + 1))
                            r.append("{} {}{}".format(*contained).lstrip())

                    # 0 is const or empty
                    # 1 is stl format string
                    # whatever after are its arguments
                    # -1 is ptr or reference
                    r = [
                        "{} {}{}".format(
                            r[0],
                            r[1].format(*r[2:]),
                            s.pop(-1)
                        ).lstrip()
                    ]

                    continue

            return r[0]

        return flatten_and_expand(ATUtils.split_stl_type(stl_t))


@dataclass
class ArgType:
    """An argument type."""

    type: str
    name: str = ""
    reg: str = ""

    def __post_init__(self):
        if self.name == "":
            raise Exception("ArgType 'name' must be provided!")

        if "std::" in self.type:
            self.type = ATUtils.format_ptr(ATUtils.expand_stl_type(self.type))

    @property
    def stripped_type(self) -> str:
        """Type stripped from const, reference and pointer.

        Returns:
            str
        """
        return ATUtils.strip_crp(self.type)

    def __str__(self) -> str:
        if self.name == "":
            return self.type

        return f"""{self.type} {self.name}{
            f"@<{self.reg}>" if self.reg else ""
        }"""

    def __eq__(self, value: object) -> bool:
        if isinstance(value, str):
            return self.type == value
        elif is_dataclass(value):
            return self.__dataclass_fields__ == value.__dataclass_fields__

        return False


class RetType(ArgType):
    """A return type."""

    def __post_init__(self):
        if "std::" in self.type:
            self.type = ATUtils.format_ptr(ATUtils.expand_stl_type(self.type))
