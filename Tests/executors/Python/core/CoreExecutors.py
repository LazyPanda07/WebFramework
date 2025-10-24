from multiprocessing.reduction import register

from web_framework_api import *

from TextGenerator import TextGenerator
from CustomFunction import CustomFunction


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
        request.register_wfdp_function("customFunction", CustomFunction)

    def do_delete(self, request, response):
        request.unregister_wfdp_function("custom_function")


class ChunksExecutor(HeavyOperationStatelessExecutor):
    def do_get(self, request, response):
        generator = TextGenerator("Some information here")

        request.send_chunks(response, generator)

    def do_post(self, request, response):
        generator = TextGenerator("Some information here")

        request.send_file_chunks(response, "file.txt", generator)


class DownloadExecutor(HeavyOperationStatelessExecutor):
    def do_get(self, request, response):
        request.stream_file("index.html", response, "index.html")


class DynamicResources(HeavyOperationStatelessExecutor):
    def do_get(self, request, response):
        file_data = request.get_file("page.md")

        response.set_body(request.process_static_file(file_data, ".md"))

    def do_post(self, request, response):
        file_data = request.get_file("print.wfdp")
        variables = {
            "data": request.get_json()["data"]
        }

        response.set_body(request.process_wfdp_file(file_data, variables))


class UploadChunkedExecutor(HeavyOperationStatelessExecutor):
    def do_post(self, request, response):
        with open(request.get_headers()["File-Name"], "wb") as stream:
            for chunk in request.get_chunks():
                stream.write(chunk.encode("ascii"))

        response.set_response_code(ResponseCodes.CREATED)
        response.set_body("Finish uploading file")


class UploadMultipartExecutor(HeavyOperationStatelessExecutor):
    def do_post(self, request, response):
        for multipart in request.get_multiparts():
            with open(multipart.get_name(), "wb") as file:
                file.write(multipart.get_data().encode("ascii"))

        response.set_response_code(ResponseCodes.CREATED)
        response.set_body("Finish uploading file")


class UploadOctetStreamExecutor(StatefulExecutor):
    def __init__(self):
        super().__init__()

        self._stream = None

    def do_post(self, request, response):
        data, last = request.get_large_data()

        if self._stream is None:
            self._stream = open(request.get_headers()["File-Name"], "wb")

        self._stream.write(data)

        if last:
            self._stream.close()

            self._stream = None

            response.set_response_code(ResponseCodes.CREATED)
            response.set_body("Finish uploading file")
