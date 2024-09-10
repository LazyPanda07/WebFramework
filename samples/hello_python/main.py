from web_framework_api.WebFramework import WebFramework # Server
from web_framework_api.utility.DLLHandler import initialize_web_framework # WebFramework initialization 
from web_framework_api.exceptions.WebFrameworkException import WebFrameworkException # Exception

if __name__ == '__main__':
    try:
        initialize_web_framework("WebFramework") # Load WebFramework shared library
        
        server = WebFramework.from_path("configs/config.json") # Create server
        
        server.start(True) # Wait
    except WebFrameworkException as exception:
        print(exception)
        
        exit(-1)
