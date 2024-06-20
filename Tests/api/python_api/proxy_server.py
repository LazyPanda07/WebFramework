import argparse

from framework.Service import Service
from framework.WebFrameworkException import WebFrameworkException

if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    parser.add_argument("--config", required=True, type=str)
    parser.add_argument("--port", required=True, type=int)
    parser.add_argument("--useHTTPS", action=argparse.BooleanOptionalAction, default=False)

    args = parser.parse_args()

    try:
        service = Service("WebFramework")
        config = service.create_config(args.config)

        config.override_configuration("useHTTPS", args.useHTTPS, True)
        config.override_configuration("port", args.port, True)

        server = service.create_web_framework_from_config(config)

        server.start(True)
    except WebFrameworkException as exception:
        print(exception)

        exit(-1)
