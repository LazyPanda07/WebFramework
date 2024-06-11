import os

from multipledispatch import dispatch

from .DLLHandler import DLLHandler
from .WebFramework import WebFramework


class Service:
    @staticmethod
    def create_service(path_to_dll: str):
        return Service(path_to_dll)

    def __init__(self, path_to_dll: str):
        path_to_dll = os.path.abspath(path_to_dll)

        if not os.path.exists(path_to_dll):
            raise FileNotFoundError(f"Path {path_to_dll} doesn't exist")

        self.__handler = DLLHandler(path_to_dll)

    @dispatch(str)
    def create_web_framework(self, config_path: str):
        config_path = os.path.abspath(config_path)

        if not os.path.exists(config_path):
            raise FileNotFoundError(f"Path {config_path} doesn't exist")

        return WebFramework.from_path(self.__handler, config_path)

    @dispatch(str, str)
    def create_web_framework(self, server_configuration: str, sources_path: str):
        return WebFramework.from_string(self.__handler, server_configuration, sources_path)
