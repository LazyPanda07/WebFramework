import unittest
from typing import List

from framework.WebFramework import initialize_web_framework
from framework.Config import Config


class APITest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        initialize_web_framework("WebFramework")

        cls.config = Config.from_path("multi_threaded_config.json")
        cls.value: List[str] = ["load_balancer_web.json"]

    def test(self):
        self.assertEqual(self.config.get_configuration(), self.config.get_raw_configuration())

        self.config.override_configuration_string_array("settingsPaths", self.value, True)

        self.assertNotEqual(self.config.get_configuration(), self.config.get_raw_configuration())


if __name__ == '__main__':
    unittest.main()
