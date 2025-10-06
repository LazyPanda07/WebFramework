from web_framework_api import *

def print_server_state():
   print("Server is running at http://127.0.0.1:8080")

if __name__ == '__main__':
    try:
        initialize_web_framework("WebFramework")  # Load WebFramework shared library

        config = Config("configs/config.json")  # Load config.json

        config.override_base_path("executors")  # Override base path for loading executors

        server = WebFramework(config)  # Create server

        server.start(True, print_server_state)  # Start server and wait
    except WebFrameworkException as exception:
        print(exception)

        exit(-1)
