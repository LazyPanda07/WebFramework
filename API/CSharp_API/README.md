C++ HTTP/HTTPS server with C# API

* [Quick start](#quick-start)
  * [Settings](#settings)
  * [Config](#config)
  * [Main.cs](#maincs)
  * [Run sample](#run-sample)
* [Executors](#executors)
  * [Hello executor](#hello-executor)


## Quick start
Server needs few files to run. [Settings file](#settings) with routes and [executors](#executors). [Config file](#config) with server settings.  
For quick start project also you will need [executor](#hello-executor) shared library.  
config.json, web.json, hello_executor.dll or libhello_executor.so files must be in the same directory as executable


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


### Main.cs
```cs
using Framework;
using Framework.Exceptions;
using Framework.Utility;

namespace hello_csharp
{
	internal class Program
	{
		static void Main(string[] args)
		{
			try
			{
				using WebFramework server = new("config.json"); // Create server

				server.Start(true); // Start server and wait
			}
			catch (WebFrameworkException e)
			{
				Console.WriteLine(e.Message);

				Environment.Exit(-1);
			}
			catch (Exception e)
			{
				Console.WriteLine(e.Message);

				Environment.Exit(-2);
			}
		}
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
  * [Windows](https://github.com/LazyPanda07/WebFramework/releases/download/Assets/windows.zip)
  * [Linux](https://github.com/LazyPanda07/WebFramework/releases/download/Assets/linux.zip)