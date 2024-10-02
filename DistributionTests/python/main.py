from web_framework_api.WebFramework import WebFramework  # Server
from web_framework_api.utility.DLLHandler import initialize_web_framework  # WebFramework initialization
from web_framework_api.exceptions.WebFrameworkException import WebFrameworkException  # Exception


def on_start():
    with open("start.txt", "w") as file:
        file.write("Server is running")

    print("Server is running")


if __name__ == '__main__':
    try:
        initialize_web_framework()

        server = WebFramework.from_path("config.json")

        server.start(True, on_start)
    except WebFrameworkException as exception:
        print(exception)

        exit(-1)
