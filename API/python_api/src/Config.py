import ctypes
from typing import List

from multipledispatch import dispatch

from .DLLHandler import DLLHandler
from .WebFrameworkException import WebFrameworkException


class Config:
    def __init__(self, handler: DLLHandler, implementation: ctypes.c_void_p):
        self.__handler = handler
        self.implementation = implementation

    @classmethod
    def from_path(cls, handler: DLLHandler, config: str):
        exception = ctypes.c_void_p(0)
        implementation = handler.call_function("createConfigFromPath", ctypes.c_void_p,
                                               ctypes.c_char_p(config.encode()), ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(handler, exception.value)

        return cls(handler, implementation)

    @classmethod
    def from_string(cls, handler, server_configuration: str, sources_path: str):
        exception = ctypes.c_void_p(0)
        implementation = handler.call_function("createConfigFromString", ctypes.c_void_p,
                                               ctypes.c_char_p(server_configuration.encode()),
                                               ctypes.c_char_p(sources_path.encode()), ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(handler, exception.value)

        return cls(handler, implementation)

    def override_configuration(self, key: str, value: str, recursive: bool):
        exception = ctypes.c_void_p(0)

        self.__handler.call_function_member_function("overrideConfigurationString", None, self.implementation,
                                                     ctypes.c_char_p(key.encode()), ctypes.c_char_p(value.encode()),
                                                     recursive, ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(self.__handler, exception.value)

    @dispatch(str, int, bool)
    def override_configuration(self, key: str, value: int, recursive: bool):
        exception = ctypes.c_void_p(0)

        self.__handler.call_function_member_function("overrideConfigurationInteger", None, self.implementation,
                                                     ctypes.c_char_p(key.encode()), ctypes.c_int64(value),
                                                     recursive, ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(self.__handler, exception.value)

    @dispatch(str, bool, bool)
    def override_configuration(self, key: str, value: bool, recursive: bool):
        exception = ctypes.c_void_p(0)

        self.__handler.call_function_member_function("overrideConfigurationBoolean", None, self.implementation,
                                                     ctypes.c_char_p(key.encode()), value,
                                                     recursive, ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(self.__handler, exception.value)

    @dispatch(str, List[str], bool)
    def override_configuration(self, key: str, value: List[str], recursive: bool):
        exception = ctypes.c_void_p(0)

        data = (ctypes.c_char_p * len(value))()

        for i in range(len(value)):
            data[i] = value[i].encode()

        self.__handler.call_function_member_function("overrideConfigurationStringArray", None, self.implementation,
                                                     ctypes.c_char_p(key.encode()), ctypes.pointer(data),
                                                     recursive, ctypes.c_int64(len(value)), ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(self.__handler, exception.value)

    @dispatch(str, List[int], bool)
    def override_configuration(self, key: str, value: List[int], recursive: bool):
        exception = ctypes.c_void_p(0)

        data = (ctypes.c_int64 * len(value))()

        for i in range(len(value)):
            data[i] = value[i]

        self.__handler.call_function_member_function("overrideConfigurationIntegerArray", None, self.implementation,
                                                     ctypes.c_char_p(key.encode()), ctypes.pointer(data),
                                                     recursive, ctypes.c_int64(len(value)), ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(self.__handler, exception.value)

    def get_configuration(self) -> str:
        exception = ctypes.c_void_p(0)

        result = self.__handler.call_function_member_function("getConfiguration", ctypes.c_char_p, self.implementation,
                                                              ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(self.__handler, exception.value)

        return result.decode()
