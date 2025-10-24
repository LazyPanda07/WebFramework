from web_framework_api import *


class CustomFunction(DynamicFunction):
    def __call__(self, *args):
        return f"Data: {args[0]} {args[1]} {args[2]}"
