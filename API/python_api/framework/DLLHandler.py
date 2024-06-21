import ctypes


class DLLHandler:
    def __init__(self, path_to_dll: str):
        self.__handle = ctypes.CDLL(path_to_dll)

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
