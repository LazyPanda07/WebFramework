import sys

from framework.Service import Service
from framework.WebFrameworkException import WebFrameworkException

if __name__ == '__main__':
    try:
        service = Service("WebFramework")
        server = service.create_web_framework(sys.argv[1])

        server.start(True)
    except WebFrameworkException as exception:
        print(exception)

        exit(-1)
