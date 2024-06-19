import argparse

from framework.Service import Service
from framework.WebFrameworkException import WebFrameworkException

if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    parser.add_argument("--config", required=True)
    parser.add_argument("--port", required=True)
    parser.add_argument("--useHTTPS")

    args = parser.parse_args()

    try:
        service = Service("WebFramework")
        config = service.create_config(args.config)

        config.override_configuration("useHTTPS", bool(args.useHTTPS), True)
        config.override_configuration("port", int(args.port), True)

        server = service.create_web_framework(config)

        server.start(True)
    except WebFrameworkException as exception:
        print(exception)

        exit(-1)
