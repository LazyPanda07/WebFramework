import sys
import asyncio
import subprocess

from framework.Service import Service
from framework.WebFrameworkException import WebFrameworkException


async def start_server():
    try:
        service = Service("WebFramework")
        server = service.create_web_framework(sys.argv[1])

        server.start(True)
    except WebFrameworkException as exception:
        print(exception)

        exit(-1)


if __name__ == '__main__':
    asyncio.run(start_server())

    exit(subprocess.run(["Core", sys.argv[1]]).returncode)
