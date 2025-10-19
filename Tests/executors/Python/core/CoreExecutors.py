from web_framework_api import *

from TextGenerator import TextGenerator


class HelloExecutor(StatelessExecutor):
    def __init__(self):
        super().__init__()
        self._value = 0

    def init(self, settings):
        self._value: int = settings.get_init_parameters()["number"]

    def do_get(self, request, response):
        response.set_body({
            "message": "Hello, World!",
            "number": self._value
        })

    def do_post(self, request, response):
        self.do_get(request, response)

    def do_head(self, request, response):
        self.do_get(request, response)

    def do_put(self, request, response):
        self.do_get(request, response)

    def do_delete(self, request, response):
        self.do_get(request, response)

    def do_patch(self, request, response):
        self.do_get(request, response)

    def do_connect(self, request, response):
        self.do_get(request, response)


class LocalizationExecutor(StatelessExecutor):
    def do_get(self, request, response):
        response.set_body({
            "result": get_localized_string("LocalizationData", "key", request.get_json()["language"])
        })


class AssetsExecutor(StatelessExecutor):
    def do_get(self, request, response):
        request.send_wfdp_file(f"{request.get_json()["fileName"]}.wfdp", response, request.get_query_parameters())

    def do_post(self, request, response):
        # TODO: register custom function
        pass

    def do_delete(self, request, response):
        request.unregister_wfdp_function("custom_function")


class ChunksExecutor(HeavyOperationStatelessExecutor):
    def do_get(self, request, response):
        generator = TextGenerator("Some information here")

        request.send_chunks(response, generator)

    def do_post(self, request, response):
        generator = TextGenerator("Some information here")

        request.send_file_chunks(response, "file.txt", generator)
