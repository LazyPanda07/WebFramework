C++ HTTP/HTTPS server with Python API

* [Quick start](#quick-start)
  * [Settings](#settings)
  * [Config](#config)
  * [main.py](#mainpy)
  * [Run sample](#run-sample)
* [Executors](#executors)
  * [Hello executor](#hello-executor)


## Quick start
Server needs few files to run. [Settings file](#settings) with routes and [executors](#executors). [Config file](#config) with server settings.  
For quick start project also you will need [executor](#hello-executor) shared library.  
Project structure:
* main.py
* config.json
* web.json
* hello_executor.dll or libhello_executor.so


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


### main.py
```python
from web_framework_api.WebFramework import WebFramework  # Server
from web_framework_api.utility.DLLHandler import initialize_web_framework  # WebFramework initialization 
from web_framework_api.exceptions.WebFrameworkException import WebFrameworkException  # Exception

if __name__ == '__main__':
  try:
    initialize_web_framework()  # Load WebFramework shared library

    server = WebFramework.from_path("config.json")  # Create server

    server.start(True)  # Wait
  except WebFrameworkException as exception:
    print(exception)

    exit(-1)
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
  * [Windows](https://github.com/LazyPanda07/WebFramework/releases/download/Assets/windows.zip)
  * [Linux](https://github.com/LazyPanda07/WebFramework/releases/download/Assets/linux.zip)
