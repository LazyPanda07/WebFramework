import argparse
import os
from typing import List
from functools import partial

from web_framework_api import *


def on_server_start(port: int):
    with open(f"start_load_balancer_{port}_server.txt", "w") as file:
        file.write(f"{os.getpid()}")


if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    parser.add_argument("--config", required=True, type=str)
    parser.add_argument("--port", required=True, type=int)
    parser.add_argument("--webServerType", type=str)
    parser.add_argument("--type", type=str)
    parser.add_argument("--custom_heuristic", action=argparse.BooleanOptionalAction, default=False)
    parser.add_argument("--serversHTTPS", action=argparse.BooleanOptionalAction, default=False)

    args = parser.parse_args()

    try:
        initialize_web_framework("WebFramework")

        config = Config(args.config)

        if args.custom_heuristic:
            config.override_configuration("name", "CustomHeuristic", True)

        if args.type == "server":
            settings_paths: List[str] = ["load_balancer_web.json"]

            config.override_configuration("webServerType", "multiThreaded", True)

            config.override_configuration("settingsPaths", settings_paths, True)
        else:
            servers_https = args.serversHTTPS
            list_of_serves: List[int] = []

            config.override_configuration("serversHTTPS", servers_https, True)

            if servers_https:
                list_of_serves = [10002, 10003]
            else:
                list_of_serves = [10000, 10001]

            config.override_configuration("127.0.0.1", list_of_serves, True)

        config.override_configuration("port", args.port, True)

        server = WebFramework(config)

        server.start(True, partial(on_server_start, args.port))
    except WebFrameworkException as exception:
        print(exception)

        exit(-1)
