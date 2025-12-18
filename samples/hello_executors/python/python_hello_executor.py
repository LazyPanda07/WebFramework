from web_framework_api import *


class PythonHelloExecutor(StatelessExecutor):
    def do_get(self, request, response):
        response.set_body({
            "message": "Hello from Python"
        })
