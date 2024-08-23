import argparse

from web_framework_api.WebFramework import WebFramework
from web_framework_api.utility.DLLHandler import initialize_web_framework
from web_framework_api.exceptions.WebFrameworkException import WebFrameworkException
from web_framework_api.utility.Config import Config

if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    parser.add_argument("--config", required=True, type=str)
    parser.add_argument("--port", required=True, type=int)
    parser.add_argument("--useHTTPS", action=argparse.BooleanOptionalAction, default=False)

    args = parser.parse_args()

    try:
        initialize_web_framework("WebFramework")

        config = Config.from_path(args.config)

        config.override_configuration("useHTTPS", args.useHTTPS, True)
        config.override_configuration("port", args.port, True)

        server = WebFramework.from_config(config)

        server.start(True)
    except WebFrameworkException as exception:
        print(exception)

        exit(-1)
