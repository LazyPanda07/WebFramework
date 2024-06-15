import ctypes

from .DLLHandler import DLLHandler


class WebFrameworkException(Exception):
    def __init__(self, handler: DLLHandler, implementation: ctypes.c_void_p):
        self.__handler = handler
        self.__implementation = implementation

    def __str__(self) -> str:
        return self.__handler.call_function("getErrorMessage", ctypes.c_char_p,
                                            ctypes.c_uint64(self.__implementation)).decode()

    def __del__(self):
        self.__handler.free(self.__implementation)
