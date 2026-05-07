import os

from .web_framework_api import *

flag = os.getenv("WEB_FRAMEWORK_DEFAULT_PATH")

if flag is not None:
    initialize_web_framework()
