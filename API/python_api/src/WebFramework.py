import ctypes
from typing import Callable

from . import Config
from .DLLHandler import DLLHandler
from .WebFrameworkException import WebFrameworkException


class WebFramework:
    def __init__(self, handler: DLLHandler, implementation: ctypes.c_void_p):
        self.__handler = handler
        self.__implementation = implementation
        self.__function_signature = ctypes.CFUNCTYPE(None)
        self.__on_start_server = None

    @classmethod
    def from_path(cls, handler: DLLHandler, config: str):
        exception = ctypes.c_void_p(0)
        implementation = handler.call_function("createWebFrameworkFromPath", ctypes.c_void_p,
                                               ctypes.c_char_p(config.encode()), ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(handler, exception.value)

        return cls(handler, implementation)

    @classmethod
    def from_string(cls, handler, server_configuration: str, sources_path: str):
        exception = ctypes.c_void_p(0)
        implementation = handler.call_function("createWebFrameworkFromString", ctypes.c_void_p,
                                               ctypes.c_char_p(server_configuration.encode()),
                                               ctypes.c_char_p(sources_path.encode()), ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(handler, exception.value)

        return cls(handler, implementation)

    @classmethod
    def from_config(cls, handler: DLLHandler, config: Config):
        exception = ctypes.c_void_p(0)
        implementation = handler.call_function("createWebFrameworkFromConfig", ctypes.c_void_p,
                                               ctypes.c_uint64(config.implementation), ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(handler, exception.value)

        return cls(handler, implementation)

    def start(self, wait: bool = False, on_start_server: Callable[[None], None] = None):
        if on_start_server is None:
            def default_function():
                pass

            on_start_server = default_function

        self.__on_start_server = on_start_server
        exception = ctypes.c_void_p(0)

        self.__handler.call_function_member_function("startWebFrameworkServer", None, self.__implementation, wait,
                                                     self.__function_signature(self.__on_start_server),
                                                     ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(self.__handler, exception.value)

    def stop(self, wait: bool = True):
        exception = ctypes.c_void_p(0)
        self.__handler.call_function_member_function("stopWebFrameworkServer", None, self.__implementation, wait,
                                                     ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(self.__handler, exception.value)

    def __del__(self):
        self.__handler.free(self.__implementation)
