import ctypes

from web_framework_api.utility.DLLHandler import DLLHandler


class WebFrameworkException(Exception):
    """
    Exception class for WebFramework exceptions
    """
    def __init__(self, implementation: ctypes.c_void_p):
        self.__implementation = implementation

    def __str__(self) -> str:
        return DLLHandler.get_instance().call_function("getErrorMessage", ctypes.c_char_p,
                                                       ctypes.c_uint64(self.__implementation)).decode()

    def __del__(self):
        DLLHandler.get_instance().delete_web_framework_exception(self.__implementation)
