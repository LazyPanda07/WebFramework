import os
import platform

from multipledispatch import dispatch
from pathlib import Path

from . import Config
from .DLLHandler import DLLHandler
from .WebFramework import WebFramework


class Service:
    @staticmethod
    def create_service(path_to_dll: str):
        return Service(path_to_dll)

    def __init__(self, path_to_dll: str):
        path_to_dll = os.path.abspath(path_to_dll)

        if platform.system() == "Windows":
            path_to_dll = f"{path_to_dll}.dll"
        else:
            path = Path(path_to_dll)
            path_to_dll = f"{path.parent}/lib{path.name}.so"

        if not os.path.exists(path_to_dll):
            raise FileNotFoundError(f"Path {path_to_dll} doesn't exist")

        self.__handler = DLLHandler(path_to_dll)

    @dispatch(str)
    def create_web_framework(self, config_path: str) -> WebFramework:
        config_path = os.path.abspath(config_path)

        if not os.path.exists(config_path):
            raise FileNotFoundError(f"Path {config_path} doesn't exist")

        return WebFramework.from_path(self.__handler, config_path)

    @dispatch(str, str)
    def create_web_framework(self, server_configuration: str, sources_path: str) -> WebFramework:
        return WebFramework.from_string(self.__handler, server_configuration, sources_path)

    @dispatch(Config)
    def create_web_framework(self, config: Config) -> WebFramework:
        return WebFramework.from_config(self.__handler, config)
