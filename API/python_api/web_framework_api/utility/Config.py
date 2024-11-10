import ctypes
from typing import List

from multipledispatch import dispatch

from web_framework_api.utility.DLLHandler import DLLHandler
from web_framework_api.exceptions.WebFrameworkException import WebFrameworkException


class Config:
    """
    Config file representation
    """

    def __init__(self, implementation: ctypes.c_void_p):
        self.implementation = implementation

    @classmethod
    def from_path(cls, config_path: str) -> "Config":
        """

        :param config_path: Path to *.json config file
        :return:
        """
        exception = ctypes.c_void_p(0)
        implementation = DLLHandler.get_instance().call_function("createConfigFromPath", ctypes.c_void_p,
                                                                 ctypes.c_char_p(config_path.encode()),
                                                                 ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        return cls(implementation)

    @classmethod
    def from_string(cls, server_configuration: str, application_directory: str) -> "Config":
        """

        :param server_configuration: *.json config file content
        :param application_directory: Working directory
        :return:
        """
        exception = ctypes.c_void_p(0)
        implementation = DLLHandler.get_instance().call_function("createConfigFromString", ctypes.c_void_p,
                                                                 ctypes.c_char_p(server_configuration.encode()),
                                                                 ctypes.c_char_p(application_directory.encode()),
                                                                 ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        return cls(implementation)

    @dispatch(str, str, bool)
    def override_configuration(self, key: str, value: str, recursive: bool = True) -> "Config":
        """
        Override string
        :param key: JSON key
        :param value: New string value
        :param recursive: Recursive search for key
        :return: self
        """
        exception = ctypes.c_void_p(0)

        DLLHandler.get_instance().call_class_member_function("overrideConfigurationString", None, self.implementation,
                                                             ctypes.c_char_p(key.encode()),
                                                             ctypes.c_char_p(value.encode()),
                                                             recursive, ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        return self

    @dispatch(str, int, bool)
    def override_configuration(self, key: str, value: int, recursive: bool = True) -> "Config":
        """
        Override integer
        :param key: JSON key
        :param value: New integer value
        :param recursive: Recursive search for key
        :return: self
        """
        exception = ctypes.c_void_p(0)

        DLLHandler.get_instance().call_class_member_function("overrideConfigurationInteger", None, self.implementation,
                                                             ctypes.c_char_p(key.encode()), ctypes.c_int64(value),
                                                             recursive, ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        return self

    @dispatch(str, bool, bool)
    def override_configuration(self, key: str, value: bool, recursive: bool = True) -> "Config":
        """
        Override bool
        :param key: JSON key
        :param value: New bool value
        :param recursive: Recursive search for key
        :return: self
        """
        exception = ctypes.c_void_p(0)

        DLLHandler.get_instance().call_class_member_function("overrideConfigurationBoolean", None, self.implementation,
                                                             ctypes.c_char_p(key.encode()), value,
                                                             recursive, ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        return self

    def override_configuration_string_array(self, key: str, value: List[str], recursive: bool = True) -> "Config":
        """
        Override string array
        :param key: JSON key
        :param value: New string array value
        :param recursive: Recursive search for key
        :return: self
        """
        exception = ctypes.c_void_p(0)

        data = (ctypes.c_char_p * len(value))()

        for i in range(len(value)):
            data[i] = value[i].encode()

        DLLHandler.get_instance().call_class_member_function("overrideConfigurationStringArray", None,
                                                             self.implementation,
                                                             ctypes.c_char_p(key.encode()), ctypes.pointer(data),
                                                             recursive, ctypes.c_int64(len(value)),
                                                             ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        return self

    def override_configuration_integer_array(self, key: str, value: List[int], recursive: bool = True) -> "Config":
        """
        Override integer array
        :param key: JSON key
        :param value: New integer array value
        :param recursive: Recursive search for key
        :return: self
        """
        exception = ctypes.c_void_p(0)

        data = (ctypes.c_int64 * len(value))()

        for i in range(len(value)):
            data[i] = value[i]

        DLLHandler.get_instance().call_class_member_function("overrideConfigurationIntegerArray", None,
                                                             self.implementation,
                                                             ctypes.c_char_p(key.encode()), ctypes.pointer(data),
                                                             recursive, ctypes.c_int64(len(value)),
                                                             ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        return self

    def override_base_path(self, base_path: str) -> "Config":
        """
        Override config file directory
        :param base_path: New base path
        :return: self
        """

        exception = ctypes.c_void_p(0)

        DLLHandler.get_instance().call_class_member_function("overrideBasePath", None, self.implementation,
                                                             ctypes.c_char_p(base_path.encode()),
                                                             ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        return self

    def get_configuration_string(self, key: str, recursive: bool = True) -> str:
        """
        Get string from config
        :param key: Config key
        :param recursive: Search recursively
        :return: Config string value
        """

        exception = ctypes.c_void_p(0)

        handler = DLLHandler.get_instance()
        string_implementation = handler.call_class_member_function("getConfigurationString",
                                                                   ctypes.c_void_p,
                                                                   self.implementation,
                                                                   ctypes.c_char_p(key.encode()),
                                                                   recursive, ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        result_ptr = handler.call_function("getDataFromString", ctypes.c_char_p,
                                           ctypes.c_uint64(string_implementation))

        result = str(result_ptr.decode())

        handler.delete_web_framework_string(string_implementation)

        return result

    def get_configuration_integer(self, key: str, recursive: bool = True) -> int:
        """
        Get integer from config
        :param key: Config key
        :param recursive: Search recursively
        :return: Config integer value
        """

        exception = ctypes.c_void_p(0)

        result = DLLHandler.get_instance().call_class_member_function("getConfigurationInteger",
                                                                      ctypes.c_int64,
                                                                      self.implementation,
                                                                      ctypes.c_char_p(key.encode()),
                                                                      recursive, ctypes.byref(exception))
        if exception:
            raise WebFrameworkException(exception.value)

        return result

    def get_configuration_boolean(self, key: str, recursive: bool = True) -> bool:
        """
        Get boolean from config
        :param key: Config key
        :param recursive: Search recursively
        :return: Config boolean value
        """

        exception = ctypes.c_void_p(0)

        result = DLLHandler.get_instance().call_class_member_function("getConfigurationBoolean",
                                                                      ctypes.c_bool,
                                                                      self.implementation,
                                                                      ctypes.c_char_p(key.encode()),
                                                                      recursive, ctypes.byref(exception))
        if exception:
            raise WebFrameworkException(exception.value)

        return result

    def get_configuration(self) -> str:
        """
        Get current config JSON string data
        :return:
        """
        exception = ctypes.c_void_p(0)

        handler = DLLHandler.get_instance()
        string_implementation = handler.call_class_member_function("getConfiguration", ctypes.c_void_p,
                                                                   self.implementation,
                                                                   ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        result_ptr = handler.call_function("getDataFromString", ctypes.c_char_p,
                                           ctypes.c_uint64(string_implementation))

        result = str(result_ptr.decode())

        handler.delete_web_framework_string(string_implementation)

        return result

    def get_base_path(self) -> str:
        """
        Get config file directory
        :return:
        """
        exception = ctypes.c_void_p(0)

        handler = DLLHandler.get_instance()
        string_implementation = handler.call_class_member_function("getBasePath", ctypes.c_void_p,
                                                                   self.implementation,
                                                                   ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        result_ptr = handler.call_function("getDataFromString", ctypes.c_char_p,
                                           ctypes.c_uint64(string_implementation))

        result = str(result_ptr.decode())

        handler.delete_web_framework_string(string_implementation)

        return result

    def get_raw_configuration(self) -> str:
        """
        Get raw config JSON string data
        :return:
        """
        exception = ctypes.c_void_p(0)

        result = DLLHandler.get_instance().call_class_member_function("getRawConfiguration", ctypes.c_char_p,
                                                                      self.implementation,
                                                                      ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        return result.decode()

    def __del__(self):
        DLLHandler.get_instance().delete_web_framework_config(self.implementation)
