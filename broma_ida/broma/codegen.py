from pathlib import Path

from pybroma import Class, Root

from broma_ida.broma.constants import BROMA_PLATFORMS
from broma_ida.class_builder.class_builder import ClassBuilder


class BromaCodegen:
    """Codegen for Broma"""

    FILE_HEADER = """// Generated by BromaIDA, do not modify
#define {BROMAIDA_PLATFORM_MACRO_NAME}
// platform shorthands
#define BROMAIDA_IS_PLATFORM_MACHO {BROMAIDA_IS_PLATFORM_MACHO_VALUE}
#define BROMAIDA_IS_PLATFORM_ANDROID {BROMAIDA_IS_PLATFORM_ANDROID_VALUE}

#define STR_CONCAT_WRAPPER(a, b) a ## b
#define STR_CONCAT(a, b) STR_CONCAT_WRAPPER(a, b)
#define PAD(size) unsigned char STR_CONCAT(__pad, __LINE__)[size]\n\n"""

    _classes: dict[str, Class]
    _path: Path
    _target_platform: BROMA_PLATFORMS
    _broma_path: Path

    def __init__(
        self,
        platform: BROMA_PLATFORMS,
        broma_classes: dict[str, Class],
        path: Path,
        broma_path: Path
    ):
        self._target_platform = platform
        self._classes = broma_classes
        self._path = path
        self._broma_path = broma_path

        (self._path / "codegen").mkdir(parents=True, exist_ok=True)

    def write(self):
        """Dumps codegenned classes, structs and enums
        to the path supplied in __init__

        Args:
            path (Path)
        """
        with open(
            self._path / "codegen" / f"{self._target_platform}.hpp",
            "w",
            buffering=10 * 1024 * 1024,
        ) as f:
            f.write(
                self.FILE_HEADER.format_map({
                    "BROMAIDA_PLATFORM_MACRO_NAME":
                        self._get_bromaida_platform_macro(),
                    "BROMAIDA_IS_PLATFORM_MACHO_VALUE":
                        int(self._target_platform in ["imac", "m1", "ios"]),
                    "BROMAIDA_IS_PLATFORM_ANDROID_VALUE":
                        int(
                            self._target_platform in ["android32", "android64"]
                        )
                })
            )

            with open(self._path / "enums.hpp") as enums:
                f.write("// enums.hpp\n")
                f.writelines(enums.readlines())
                f.write("\n\n")

            with open(self._path / "stl_includes.hpp") as stl_includes:
                f.write("// stl_includes.hpp\n")
                f.writelines(stl_includes.readlines())
                f.write("\n\n")

            with open(self._path / "cocos2d_geometry.hpp") as cocos2d_geometry:
                f.write("// cocos2d_geometry.hpp\n")
                f.writelines(cocos2d_geometry.readlines())
                f.write("\n\n")

            with open(self._path / "cocos2d.hpp") as cocos:
                f.write("// cocos2d.hpp\n")
                f.writelines(self._filter_relative_includes(cocos.readlines()))
                f.write("\n\n")

            with open(self._path / "fmod.hpp") as fmod:
                f.write("// fmod.hpp\n")
                f.writelines(fmod.readlines())
                f.write("\n\n")

            with open(self._path / "helpers.hpp") as helpers:
                f.write("// helpers.hpp\n")
                f.writelines(helpers.readlines())
                f.write("\n\n")

            f.flush()

            # now we codegen the Broma file
            f.write("// Broma\n\n")

            f.write("// typdefs\n")
            f.write("enum class TodoReturn {}; // :troll:\n")
            f.write("\n")

            f.flush()

            f.write("// class fwddec\n")
            for c in self._classes.keys():
                if "::" in c:
                    split_c = c.split("::")

                    if len(split_c) == 2:
                        f.write(
                            f"namespace {split_c[0]} {{ "
                            f"class {split_c[1]}; "
                            "}}\n"
                        )
                    elif len(split_c) == 3:
                        f.write(
                            f"namespace {split_c[0]} {{ "
                            f"namespace {split_c[1]} {{ "
                            f"class {split_c[2]};"
                            "}} "
                            "}}\n"
                        )
                else:
                    f.write(f"class {c};\n")
            f.write("\n")

            f.flush()

            f.write("// extras\n")
            for broma_class in Root(
                    str(self._broma_path / "Extras.bro")
            ).classes:
                f.write(
                    ClassBuilder(self._target_platform, broma_class).get_str()
                )
            f.write("\n")

            f.flush()

            f.write("// delegates and non-polymorphic classes\n")
            for _, broma_class in self._classes.items():
                if len(broma_class.superclasses) != 0:
                    continue

                f.write(
                    ClassBuilder(self._target_platform, broma_class).get_str()
                )
            f.write("\n")

            f.flush()

            f.write("// classes\n")
            for _, broma_class in self._classes.items():
                if len(broma_class.superclasses) == 0:
                    continue

                f.write(
                    ClassBuilder(self._target_platform, broma_class).get_str()
                )
            f.write("\n\n")

            f.flush()

            # Some STL types need declaration of either cocos2d or
            # broma classes, so we put this at the very bottom
            # of the codegenned header

            with open(self._path / "stl_types.hpp") as stl_types:
                f.write("// stl_types.hpp\n")
                f.writelines(
                    self._filter_relative_includes(stl_types.readlines())
                )
                f.write("\n\n")

    def _get_bromaida_platform_macro(self) -> str:
        """Gets the BromaIDA platform macro name (shocker)"""
        plat_to_macro_suffix: dict[BROMA_PLATFORMS, str] = {
            "win": "WINDOWS",
            "imac": "INTEL_MACOS",
            "m1": "M1_MACOS",
            "ios": "IOS",
            "android32": "ANDROID32",
            "android64": "ANDROID64"
        }

        return f"""BROMAIDA_PLATFORM_{
            plat_to_macro_suffix[self._target_platform]
        }"""

    def _filter_relative_includes(self, lines: list[str]) -> list[str]:
        """Comments out relative includes from a list of lines

        Args:
            lines (list[str])

        Returns:
            list[str]
        """
        for i, e in enumerate(lines):
            if e.startswith('#include "'):
                lines[i] = f"// {lines[i]}"

        return lines
