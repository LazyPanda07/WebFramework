import time
import os

from web_framework_api import *


class IdExecutor(HeavyOperationStatelessExecutor):
    def do_get(self, request, response):
        time.sleep(10)

        response.set_body({
            "id": os.getpid()
        })
