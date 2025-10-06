import sys
import os

from web_framework_api import *


def write_process_id():
    with open("start_core_server.txt", "w") as file:
        file.write(str(os.getpid()))


if __name__ == '__main__':
    try:
        if os.path.exists("test_database.sqlite"):
            os.remove("test_database.sqlite")

        initialize_web_framework("WebFramework")

        server = WebFramework(sys.argv[1])

        server.start(True, write_process_id)
    except WebFrameworkException as exception:
        print(exception)

        exit(-1)
