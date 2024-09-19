from pybroma import Class

from broma_ida.broma.constants import BROMA_PLATFORMS


class ClassBuilder:
    """Builds a C++ class"""
    _class_str: str
    _broma_class: Class
    _target_platform: BROMA_PLATFORMS

    def _import_class(self) -> None:
        """Converts a Binding to a class string"""
        self._class_str = f"class {self._broma_class.name}"

        if len(self._broma_class.superclasses) != 0:
            self._class_str += f""" : public {
                ", ".join(self._broma_class.superclasses)
            }\n{{\npublic:\n"""
        else:
            self._class_str += "\n{\npublic:\n"

        has_left_functions: bool = False

        for field in self._broma_class.fields:
            func_field = field.getAsFunctionBindField()
            member_field = field.getAsMemberField()
            pad_field = field.getAsPadField()

            if func_field is not None and func_field.prototype.is_virtual:
                function = func_field.prototype

                self._class_str += f"""\tvirtual {
                    function.ret.name.replace("gd::", "std::")
                } {function.name}({", ".join([
                        f"{a.name.replace('gd::', 'std::')} {n}"
                        for n, a in function.args.items()])
                    });\n"""

                has_left_functions = True

            elif member_field is not None:
                if has_left_functions:
                    self._class_str += "\n"
                    has_left_functions = False

                # currently, only members use the geode namespace
                self._class_str += f"""\t{
                    member_field.type.name.replace(
                        "gd::", "std::"
                    ).replace("geode::", "")
                } {member_field.name};\n"""
            elif pad_field is not None:
                # skip other members because no padding for current platform
                if self._target_platform not in \
                        pad_field.amount.platforms_as_dict():
                    break

                pad_amount = pad_field.amount.platforms_as_dict()[
                    self._target_platform
                ]

                # thank you andy pads
                if pad_amount == 0:
                    continue

                if has_left_functions:
                    self._class_str += "\n"
                    has_left_functions = False

                self._class_str += f"""\tPAD({pad_amount});\n"""

        self._class_str += "};\n\n"

    def __init__(
        self,
        platform: BROMA_PLATFORMS,
        broma_class: Class
    ):
        self._target_platform = platform
        self._broma_class = broma_class
        self._class_str = ""

        self._import_class()

    def get_str(self):
        return self._class_str
