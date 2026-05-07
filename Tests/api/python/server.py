import sys
import os

from web_framework_api import *


def print_running_state():
    print("Server is running...")


if __name__ == '__main__':
    try:
        if os.path.exists("test_database.sqlite"):
            os.remove("test_database.sqlite")

        runtimes = os.getenv("RUNTIMES")

        initialize_web_framework("WebFramework")

        config = Config(sys.argv[1])

        if runtimes is not None:
            if "python" in runtimes:
                config.override_configuration("$[]WebFramework.runtimes.0.enabled", True)

            if ".net" in runtimes:
                config.override_configuration("$[]WebFramework.runtimes.1.enabled", True)

        server = WebFramework(config)

        server.start(True, print_running_state)
    except WebFrameworkException as exception:
        print(exception)

        exit(-1)
