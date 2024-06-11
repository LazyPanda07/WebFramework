import ctypes
from typing import Callable

from .DLLHandler import DLLHandler


class WebFramework:
    def __init__(self, handler: DLLHandler, implementation: ctypes.c_void_p):
        self.__handler = handler
        self.__implementation = implementation
        self.__function_signature = ctypes.CFUNCTYPE(None)
        self.__on_start_server = None

    @classmethod
    def from_path(cls, handler: DLLHandler, config: str):
        implementation = handler.call_function("createWebFrameworkFromPath", ctypes.c_void_p,
                                               ctypes.c_char_p(config.encode()))

        return cls(handler, implementation)

    @classmethod
    def from_string(cls, handler, server_configuration: str, sources_path: str):
        implementation = handler.call_function("createWebFrameworkFromString", ctypes.c_void_p,
                                               ctypes.c_char_p(server_configuration.encode()),
                                               ctypes.c_char_p(sources_path.encode()))

        return cls(handler, implementation)

    def start(self, wait: bool = False, on_start_server: Callable[[None], None] = None):
        if on_start_server is None:
            def default_function():
                pass

            on_start_server = default_function

        self.__on_start_server = on_start_server

        self.__handler.call_function_member_function("startWebFrameworkServer", None, self.__implementation, wait,
                                                     self.__function_signature(self.__on_start_server), False)

    def stop(self, wait: bool = True):
        self.__handler.call_function_member_function("stopWebFrameworkServer", None, self.__implementation, wait)

    def __del__(self):
        self.__handler.free(self.__implementation)
