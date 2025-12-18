import time
import os
import threading

from web_framework_api import *


class CustomHeuristic(LoadBalancerHeuristic):
    _id = 0
    _id_lock = threading.Lock()

    @staticmethod
    def _get_id():
        with CustomHeuristic._id_lock:
            value = CustomHeuristic._id

            CustomHeuristic._id += 1

            return value

    def __init__(self, ip: str, port: str, use_https: bool):
        super().__init__(ip, port, use_https)
        self._id = CustomHeuristic._get_id()

    def __call__(self):
        return self._id


class IdExecutor(HeavyOperationStatelessExecutor):
    def do_get(self, request, response):
        time.sleep(10)

        response.set_body({
            "id": os.getpid()
        })
