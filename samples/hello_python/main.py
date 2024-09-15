from web_framework_api.WebFramework import WebFramework  # Server
from web_framework_api.utility.Config import Config  # Config file
from web_framework_api.utility.DLLHandler import initialize_web_framework  # WebFramework initialization
from web_framework_api.exceptions.WebFrameworkException import WebFrameworkException  # Exception

def on_start():
   print("Server is running")

if __name__ == '__main__':
    try:
        initialize_web_framework("WebFramework")  # Load WebFramework shared library

        config = Config.from_path("configs/config.json")  # Load config.json

        config.override_base_path("executors")  # Override base path for loading executors

        server = WebFramework.from_config(config)  # Create server

        server.start(True, on_start)  # Start server and wait
    except WebFrameworkException as exception:
        print(exception)

        exit(-1)
