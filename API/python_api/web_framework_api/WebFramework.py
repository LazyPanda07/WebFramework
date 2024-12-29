import ctypes
import os
from typing import Callable

from .utility import Config
from web_framework_api.utility.DLLHandler import DLLHandler
from web_framework_api.exceptions.WebFrameworkException import WebFrameworkException


class WebFramework:
    """
    Web server
    """

    @staticmethod
    def get_web_framework_version() -> str:
        """
        Get WebFramework version
        :return: String representation of version in format {major}.{minor}.{patch}
        """
        version = DLLHandler.get_instance().call_function("getWebFrameworkVersion", ctypes.c_char_p)

        return str(version.decode())

    def __init__(self, implementation: ctypes.c_void_p):
        self.__implementation = implementation
        self.__function_signature = ctypes.CFUNCTYPE(None)
        self.__on_start_server = None

    @classmethod
    def from_path(cls, config_path: str) -> "WebFramework":
        """

        :param config_path: Path to *.json config
        :return:
        """
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
    def from_string(cls, server_configuration: str, application_directory: str) -> "WebFramework":
        """

        :param server_configuration: *.json config file content
        :param application_directory: Working directory
        :return:
        """
        exception = ctypes.c_void_p(0)
        implementation = DLLHandler.get_instance().call_function("createWebFrameworkFromString", ctypes.c_void_p,
                                                                 ctypes.c_char_p(server_configuration.encode()),
                                                                 ctypes.c_char_p(application_directory.encode()),
                                                                 ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        return cls(implementation)

    @classmethod
    def from_config(cls, config: Config) -> "WebFramework":
        """

        :param config: Config instance
        :return:
        """
        exception = ctypes.c_void_p(0)
        implementation = DLLHandler.get_instance().call_function("createWebFrameworkFromConfig", ctypes.c_void_p,
                                                                 ctypes.c_uint64(config.implementation),
                                                                 ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        return cls(implementation)

    def start(self, wait: bool = False, on_start_server: Callable[[], None] = None):
        """
        Start server
        :param wait: Wait until server stop(False doesn't work)
        :param on_start_server: On start server callback
        :return:
        """
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
        """
        Stop server
        :param wait: Wait until server stop
        :return:
        """
        exception = ctypes.c_void_p(0)
        DLLHandler.get_instance().call_class_member_function("stopWebFrameworkServer", None, self.__implementation,
                                                             wait,
                                                             ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

    def is_server_running(self) -> bool:
        """
        Is server running
        :return:
        """

        exception = ctypes.c_void_p(0)
        result = DLLHandler.get_instance().call_class_member_function("isServerRunning", ctypes.c_bool,
                                                                      self.__implementation,
                                                                      ctypes.byref(exception))

        if exception:
            raise WebFrameworkException(exception.value)

        return result

    def __del__(self):
        DLLHandler.get_instance().delete_web_framework(self.__implementation)
