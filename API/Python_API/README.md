C++ HTTP/HTTPS server with Python API


* [Quick start](#quick-start)
  * [main.py](#mainpy)
  * [Settings](#settings)
  * [Config](#config)
  * [Run sample](#run-sample)
* [Executors](#executors)


## Quick start
Server needs few files to run: 
* [web.json](#settings) with routes
* [Executors](#executors)
	* [Windows](https://github.com/LazyPanda07/WebFramework/releases/latest/download/hello_executor_windows.zip)
	* [Linux](https://github.com/LazyPanda07/WebFramework/releases/latest/download/hello_executor_linux.zip)
* [config.json](#config) with server settings  
All these files must be in the same directory as ```main.py```


### main.py
```python
from web_framework_api.WebFramework import WebFramework  # Server
from web_framework_api.utility.DLLHandler import initialize_web_framework  # WebFramework initialization 
from web_framework_api.exceptions.WebFrameworkException import WebFrameworkException  # Exception

def on_start():
  print("Server is running")

if __name__ == '__main__':
  try:
    initialize_web_framework()  # Load WebFramework shared library

    server = WebFramework.from_path("config.json")  # Create server

    server.start(True, on_start)  # Start server and wait
  except WebFrameworkException as exception:
    print(exception)

    exit(-1)
```


### Settings
```web.json```
```json
{
  "HelloExecutor": {
    "route": "",
    "loadType": "initialization"
  }
}
```


### Config
```config.json```
```json
{
  "WebServer": {
    "ip": "0.0.0.0",
    "port": 8080,
    "timeout": 0
  },
  "WebFramework": {
    "settingsPaths": [
      "web.json"
    ],
    "loadSources": [
      "hello_executor"
    ],
    "assetsPath": "assets",
    "templatesPath": "templates",
    "cachingSize": 536870912,
    "webServerType": "multiThreaded",
    "HTTPS": {
      "useHTTPS": false,
      "pathToCertificate": "certificates/cert.pem",
      "pathToKey": "certificates/key.pem"
    },
    "defaultAssetsPath": "WebFrameworkAssets"
  },
  "Logging": {
    "usingLogging": false,
    "dateFormat": "DMY",
    "logFileSize": 134217728
  },
  "ThreadPoolServer": {
    "threadCount": 0
  }
}
```


### Run sample
After running server open url [127.0.0.1:8080](http://127.0.0.1:8080).  
You will see response from server
```json
{
  "message": "Hello, World!"
}
```


## Executors
Executors are C++ classes that responsible for giving responses for their route(url).  
Source code of HelloExecutor from example  
```HelloExecutor.h```
```cpp
#pragma once

#include "Executors/BaseStatelessExecutor.h"

namespace executors
{
	class HelloExecutor : public framework::BaseStatelessExecutor
	{
	public:
		HelloExecutor() = default;

		void doGet(framework::HTTPRequest& request, framework::HTTPResponse& response) override;

		~HelloExecutor() = default;
	};
}
```
```HelloExecutor.cpp```
```cpp
#include "HelloExecutor.h"

#include "JSONBuilder.h"

namespace executors
{
	void HelloExecutor::doGet(framework::HTTPRequest& request, framework::HTTPResponse& response)
	{
		response.addBody(json::JSONBuilder(CP_UTF8).appendString("message", "Hello, World!"));
	}

	DECLARE_EXECUTOR(HelloExecutor);
}
```
More information you can find in [wiki](https://github.com/LazyPanda07/WebFramework/wiki/Executors).


### Hello executor
* Links
  * [Windows](https://github.com/LazyPanda07/WebFramework/releases/latest/download/hello_executor_windows.zip)
  * [Linux](https://github.com/LazyPanda07/WebFramework/releases/latest/download/hello_executor_linux.zip)
