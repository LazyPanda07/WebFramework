import ctypes
import platform
import sys
import os

from pathlib import Path


def initialize_web_framework(path_to_dll: str = ""):
    """
    Load WebFramework shared library
    :param path_to_dll: Path to shared library without prefixes(lib for Linux) and file extensions(.dll, .so). By default, take path to WebFramework library from pip package
    :return:
    """
    if DLLHandler.instance is not None:
        return

    shared_libraries_dir = os.path.abspath(
        os.path.join(Path(__file__).parent.parent, "dll" if sys.platform == "win32" else "lib"))

    if len(path_to_dll) == 0:
        path_to_dll = os.path.join(shared_libraries_dir, "WebFramework")

    path_to_dll = os.path.abspath(path_to_dll)

    if platform.system() == "Windows":
        path_to_dll = f"{path_to_dll}.dll"

        os.environ["PATH"] += os.pathsep + f"{shared_libraries_dir}"
    else:
        path = Path(path_to_dll)
        path_to_dll = f"{path.parent}/lib{path.name}.so"

        os.environ["LD_LIBRARY_PATH"] += os.pathsep + f"{shared_libraries_dir}"

    if not os.path.exists(path_to_dll):
        raise FileNotFoundError(f"Path {path_to_dll} doesn't exist")

    DLLHandler.instance = DLLHandler(path_to_dll)


class DLLHandler:
    instance = None

    def __init__(self, path_to_dll: str):
        self.__handle = ctypes.CDLL(path_to_dll)

    @classmethod
    def get_instance(cls):
        if cls.instance is None:
            raise Exception("WebFramework must be initialized with initialize_web_framework function")

        return cls.instance

    def call_function(self, function_name: str, return_type, *args):
        function = self.__handle[function_name]

        function.restype = return_type

        return function(*args)

    def call_class_member_function(self, function_name: str, return_type, implementation: ctypes.c_void_p, *args):
        function = self.__handle[function_name]

        function.restype = return_type

        return function(ctypes.c_uint64(implementation), *args)

    def free(self, implementation: ctypes.c_void_p):
        self.call_function("deleteWebFrameworkObject", None, ctypes.c_uint64(implementation))
