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

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._id = CustomHeuristic._get_id()

    def __call__(self):
        return self._id
