import ctypes
import platform
import os

from pathlib import Path


def initialize_web_framework(path_to_dll: str):
    if DLLHandler.instance is not None:
        return

    path_to_dll = os.path.abspath(path_to_dll)

    if platform.system() == "Windows":
        path_to_dll = f"{path_to_dll}.dll"
    else:
        path = Path(path_to_dll)
        path_to_dll = f"{path.parent}/lib{path.name}.so"

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
