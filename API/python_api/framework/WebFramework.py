import ctypes
import os
import platform
from pathlib import Path
from typing import Callable

from . import Config
from .DLLHandler import DLLHandler
from .WebFrameworkException import WebFrameworkException


def initialize_web_framework(path_to_dll: str):
    if DLLHandler.__instance is not None:
        raise Exception("WebFramework already initialized")

    path_to_dll = os.path.abspath(path_to_dll)

    if platform.system() == "Windows":
        path_to_dll = f"{path_to_dll}.dll"
    else:
        path = Path(path_to_dll)
        path_to_dll = f"{path.parent}/lib{path.name}.so"

    if not os.path.exists(path_to_dll):
        raise FileNotFoundError(f"Path {path_to_dll} doesn't exist")

    DLLHandler.__instance = DLLHandler(path_to_dll)


class WebFramework:
    def __init__(self, implementation: ctypes.c_void_p):
        self.__implementation = implementation
        self.__function_signature = ctypes.CFUNCTYPE(None)
        self.__on_start_server = None

    @classmethod
    def from_path(cls, config_path: str):
        config_path = os.path.abspath(config_path)

        if not os.path.exists(config_path):
            raise FileNotFoundError(f"Path {config_path} doesn't exist")

        exception = ctypes.c_void_p(0)
        implementation = DLLHandler.get_instance().call_function("createWebFrameworkFromPath", ctypes.c_void_p,
                                                                 ctypes.c_char_p(config_path.encode()),
                                                                 ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        return cls(implementation)

    @classmethod
    def from_string(cls, server_configuration: str, sources_path: str):
        exception = ctypes.c_void_p(0)
        implementation = DLLHandler.get_instance().call_function("createWebFrameworkFromString", ctypes.c_void_p,
                                                                 ctypes.c_char_p(server_configuration.encode()),
                                                                 ctypes.c_char_p(sources_path.encode()),
                                                                 ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        return cls(implementation)

    @classmethod
    def from_config(cls, config: Config):
        exception = ctypes.c_void_p(0)
        implementation = DLLHandler.get_instance().call_function("createWebFrameworkFromConfig", ctypes.c_void_p,
                                                                 ctypes.c_uint64(config.implementation),
                                                                 ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        return cls(implementation)

    def start(self, wait: bool = False, on_start_server: Callable[[None], None] = None):
        if on_start_server is None:
            def default_function():
                pass

            on_start_server = default_function

        self.__on_start_server = on_start_server
        exception = ctypes.c_void_p(0)

        DLLHandler.get_instance().call_class_member_function("startWebFrameworkServer", None, self.__implementation,
                                                             wait,
                                                             self.__function_signature(self.__on_start_server),
                                                             ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

    def stop(self, wait: bool = True):
        exception = ctypes.c_void_p(0)
        DLLHandler.get_instance().call_class_member_function("stopWebFrameworkServer", None, self.__implementation,
                                                             wait,
                                                             ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

    def __del__(self):
        DLLHandler.get_instance().free(self.__implementation)
