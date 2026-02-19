import sys
import os

from web_framework_api import *


def print_running_state():
    print("Server is running...")


if __name__ == '__main__':
    try:
        if os.path.exists("test_database.sqlite"):
            os.remove("test_database.sqlite")

        initialize_web_framework("WebFramework")

        server = WebFramework(sys.argv[1])

        server.start(True, print_running_state)
    except WebFrameworkException as exception:
        print(exception)

        exit(-1)
