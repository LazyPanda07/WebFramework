# web_framework_flutter_api

C++ HTTP/HTTPS server with Flutter API

* [Quick start](#quick-start)
    * [main.dart](#maindart)
    * [Settings](#settings)
    * [Config](#config)
    * [Run sample](#run-sample)
* [Executors](#executors)


## Quick start
If you want to run sample on Android you will need physical device or arm64-v8a emulator.  
Server needs few files to run:
* [assets/executors/web.json](#settings) with routes
* [Executors](#executors)
    * [assets/executors/hello_executor.dll](https://github.com/LazyPanda07/WebFramework/releases/latest/download/hello_executor_windows.zip) (for Windows)
    * [assets/executors/libhello_executor.so](https://github.com/LazyPanda07/WebFramework/releases/latest/download/hello_executor_android.zip) (for Android)
* [assets/configs/config.json](#config) with server settings  


### main.dart
```dart
import 'dart:io';
import 'dart:async';

import 'package:flutter/gestures.dart';
import 'package:flutter/material.dart';

import 'package:web_framework_flutter_api/config.dart';
import 'package:web_framework_flutter_api/web_framework.dart';
import 'package:web_framework_flutter_api/web_framework_exception.dart';
import 'package:web_framework_flutter_api/web_framework_utilities.dart';

import 'package:url_launcher/url_launcher.dart';

Future<void> main() async {
  if (Platform.isAndroid) {
    await unpackAndroidAssets(); // unpack assets on Android device
  }

  runApp(const App());
}

class App extends StatefulWidget {
  const App({super.key});

  @override
  State<App> createState() => _AppState();
}

class _AppState extends State<App> {
  String _message = "Server is not running";
  String _address = "";
  WebFramework? _server;
  bool _isRunning = false;
  TapGestureRecognizer? _recognizer;

  void _initAddress(Config config) {
    String ip = config.getConfigurationString("ip"); // Get IP address from config.json
    int port = config.getConfigurationInteger("port"); // Get port from config.json
    bool useHTTPS = config.getConfigurationBoolean("useHTTPS"); // Get HTTPS setting from config.json

    if (ip == "0.0.0.0") {
      ip = "127.0.0.1";
    }

    _address = "${useHTTPS ? "https" : "http"}://$ip:$port";
  }

  Future<void> start() async {
    Config? config;

    try {
      config = await Config.fromPath("configs/config.json"); // Create Config

      config.overrideBasePath("${config.handler.assetsPath}/executors"); // Override base path for loading executors

      _server = await WebFramework.fromConfig(config); // Create server

      await _server!.start(); // Start server in isolate

      setState(() {
        _isRunning = true;

        _initAddress(config!);

        config.dispose(); // Dispose Config

        _message = "Server is running at ";
      });
    } on WebFrameworkException catch (e) {
      _message = e.toString();

      e.dispose();
      config?.dispose();
    } on Exception catch (e) {
      _message = e.toString();

      config?.dispose(); // Dispose Config
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;

    setState(() {});
  }

  Future<void> stop() async {
    try {
      if (_server == null) {
        return;
      }

      await _server!.stop(wait: true);

      setState(() {
        _isRunning = false;

        _address = "";

        _message = "Server is not running";
      });
    } on WebFrameworkException catch (e) {
      _message = e.toString();
    } on Exception catch (e) {
      _message = e.toString();
    }
  }

  @override
  Widget build(BuildContext context) {
    _recognizer?.dispose();

    if (_address.isNotEmpty) {
      _recognizer = TapGestureRecognizer()..onTap = () => launchUrl(Uri.parse(_address));
    } else {
      _recognizer = TapGestureRecognizer();
    }

    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text("Plugin example app"),
        ),
        body: Column(children: [
          Center(
            child: RichText(
                text: TextSpan(children: [
                  TextSpan(text: _message, style: const TextStyle(color: Colors.black)),
                  TextSpan(
                    text: _address,
                    style: const TextStyle(color: Colors.blue),
                    recognizer: _recognizer,
                  )
                ])),
          ),
          if (_isRunning)
            Center(
                child: TextButton(
                    onPressed: stop,
                    style: const ButtonStyle(backgroundColor: WidgetStatePropertyAll(Colors.blue), foregroundColor: WidgetStatePropertyAll(Colors.white)),
                    child: const Text("Stop server")))
          else
            Center(
                child: TextButton(
                    onPressed: start,
                    style: const ButtonStyle(backgroundColor: WidgetStatePropertyAll(Colors.blue), foregroundColor: WidgetStatePropertyAll(Colors.white)),
                    child: const Text("Start server")))
        ]),
      ),
    );
  }

  @override
  void dispose() {
    super.dispose();

    _server?.dispose(); // Dispose server
    _recognizer?.dispose();
  }
}
```


### Settings
```assets/executors/web.json```
```json
{
  "CXXHelloExecutor": {
    "route": "",
    "loadType": "initialization",
    "api": "cxx"
  }
}
```


### Config
```assets/configs/config.json```
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
      "cxx_hello_executor"
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
Build and run sample in Android Studio.  
You will see response from server
```json
{
  "message": "Hello, World!"
}
```


## Executors
Executors are C++, C, Python or C# classes that responsible for giving responses for their route(url).  
Source code of HelloExecutor from example  
```CXXHelloExecutor.h```
```cpp
#pragma once

#include <Executors/StatelessExecutor.h>

namespace executors
{
	class CXXHelloExecutor : public framework::StatelessExecutor
	{
	public:
		CXXHelloExecutor() = default;

		void doGet(framework::HttpRequest& request, framework::HttpResponse& response) override;

		~CXXHelloExecutor() = default;
	};
}
```
```CXXHelloExecutor.cpp```
```cpp
#include "CXXHelloExecutor.h"

namespace executors
{
	void CXXHelloExecutor::doGet(framework::HttpRequest& request, framework::HttpResponse& response)
	{
		response.setBody(framework::JsonBuilder().append("message", "Hello, World!"));
	}

	DECLARE_EXECUTOR(CXXHelloExecutor);
}
```
More information you can find in [wiki](https://github.com/LazyPanda07/WebFramework/wiki/Executors-API).
