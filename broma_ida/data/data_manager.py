from typing import Self, Any
from pathlib import Path
from pickle import UnpicklingError
import shelve


class DataManager:
    """
    Manages saved data. This class is a singleton.
    """

    __default_argument = object()

    __instance: Self = None  # type: ignore[assignment]
    __shelf: shelve.Shelf[Any] = None  # type: ignore[assignment]
    __shelf_path: Path = None  # type: ignore[assignment]

    def __new__(cls, *args, **kwargs) -> Self:
        if not cls._DataManager__instance:  # type: ignore[has-type]
            cls._DataManager__instance = super(DataManager, cls).__new__(
                cls, *args, **kwargs
            )

        return cls._DataManager__instance  # type: ignore[misc]

    def _init_values(self):
        """Initializes DataManager's default values"""
        try:
            self.get("always_overwrite_merge_information", True)
            self.get("disable_broma_hash_check", False)
            self.get("always_overwrite_idb", False)
            self.get("export_return_types", False)
            self.get("export_args_names", False)

            self.get("import_types", True)
            self.get("set_default_parser_args", True)
            self.get("ignore_mismatched_structs", False)
        except UnpicklingError:
            print(
                "[!] BromaDataManager: Shelf is corrupted! "
                "Resetting to default values."
            )
            self._delete_shelf()
            self.__shelf = shelve.open(self.__shelf_path)

            self._init_values()

    def init(self, filepath: Path):
        """Initializes a DataManager instance.

        Args:
            filepath (Path): Path to the shelf file
        """
        filepath.parent.mkdir(exist_ok=True)

        if self.__shelf is None:
            self.__shelf_path = filepath

            try:
                self.__shelf = shelve.open(filepath)
            except SyntaxError:
                print(
                    "[!] BromaDataManager: Shelf is corrupted! "
                    "Resetting to default values."
                )
                self._delete_shelf()
                self.__shelf = shelve.open(filepath)

            self._init_values()

    def sync(self):
        """Manually syncs changes to the shelf"""
        self.__shelf.sync()

    def has(self, key: str) -> bool:
        """Checks if the shelf has a certain key

        Args:
            key (str)

        Returns:
            bool
        """
        return key in self.__shelf

    def get(self, key: str, default: Any = __default_argument) -> Any:
        """Safely gets data from the shelf.
        Populates the key with default and returns it if the key doesnt exist.

        Args:
            key (str): The key name
            default (Any): The default value
        """
        if self.has(key):
            return self.__shelf[key]
        else:
            if default is self.__default_argument:
                raise KeyError(
                    f"Shelf has no '{key}' key and no default value provided!"
                )
            self.__shelf[key] = default
            return default

    def set(self, key: str, value: Any):
        """Sets a key to a value in the shelf.

        Args:
            key (str)
            value (Any)
        """
        self.__shelf[key] = value

    # def register_idb(self, idb_hash: str, ) -> None:
    #     """_summary_

    #     Args:
    #         idb_hash (str): _description_
    #     """

    def close(self):
        """Closes the DataManager. Saves everything to the shelf."""
        if self.__shelf is not None:
            self.__shelf.close()
            self.__shelf = None  # type: ignore[assignment]

    def _delete_shelf(self):
        """Deletes the shelf"""
        for suffix in (".bak", ".dat", ".dir"):
            self.__shelf_path.with_suffix(suffix).unlink(True)
