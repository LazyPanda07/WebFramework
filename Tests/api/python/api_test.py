import json
import pathlib
import unittest

from framework.utility.DLLHandler import initialize_web_framework
from framework.utility.Config import Config


def get_configuration():
    with open("multi_threaded_config.json", "r") as file:
        result = ""
        temp = file.read()

        for c in temp:
            if c == '\r':
                continue

        result += c

    return result


def create_config():
    return Config.from_path("multi_threaded_config.json")


class ConfigConstructors(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.config_from_path = create_config()
        cls.config_from_strings = Config.from_string(get_configuration(), str(pathlib.Path().resolve()))

    def test(self):
        self.assertEqual(self.config_from_path.get_raw_configuration(),
                         self.config_from_strings.get_raw_configuration())
        self.assertEqual(self.config_from_path.get_configuration(), self.config_from_strings.get_configuration())
        self.assertEqual(self.config_from_path.get_base_path(), self.config_from_strings.get_base_path())


class ConfigOverrideString(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.config = create_config()

    def test(self):
        self.config.override_configuration("webServerType", "threadPool", True)

        value = json.loads(self.config.get_configuration())["WebFramework"]["webServerType"]

        self.assertEqual(value, "threadPool")


class ConfigOverrideInteger(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.config = create_config()

    def test(self):
        self.config.override_configuration("cachingSize", 0, True)

        value = json.loads(self.config.get_configuration())["WebFramework"]["cachingSize"]

        self.assertEqual(value, 0)


class ConfigOverrideBool(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.config = create_config()

    def test(self):
        self.config.override_configuration("usingLogging", False, True)

        value = json.loads(self.config.get_configuration())["Logging"]["usingLogging"]

        self.assertFalse(value)


class ConfigOverrideStringArray(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.config = create_config()

    def test(self):
        self.config.override_configuration_string_array("loadSources", ["anotherSource"], True)

        value = json.loads(self.config.get_configuration())["WebFramework"]["loadSources"]

        self.assertEqual(value, ["anotherSource"])


class ConfigOverrideIntegerArray(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.config = create_config()

    def test(self):
        self.config.override_configuration_integer_array("port", [15], True)

        value = json.loads(self.config.get_configuration())["WebServer"]["port"]

        self.assertEqual(value, [15])


if __name__ == '__main__':
    initialize_web_framework("WebFramework")

    unittest.main()
