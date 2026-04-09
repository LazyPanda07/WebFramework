from web_framework_api import *


class CustomFunction(DynamicFunction):
    def __call__(self, arguments):
        data = arguments["@customFunction"]["data"]

        return f"Data: {data[0]} {data[1]} {data[2]}"
