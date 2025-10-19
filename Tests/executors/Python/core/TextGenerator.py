from web_framework_api import *


class TextGenerator(ChunkGenerator):
    def __init__(self, data: str):
        self._data = data
        self._offset = 0
        self._last_result = ""
        self._STRING_SIZE = 15

    def generate(self):
        if self._offset > len(self._data):
            return ""

        self._last_result = self._data[
            self._offset:self._offset + min(self._STRING_SIZE, len(self._data) - self._offset)]

        self._offset += len(self._last_result)

        return self._last_result
