import 'dart:io';
import 'dart:async';

import 'package:flutter/material.dart';

import 'package:web_framework_flutter_api/config.dart';
import 'package:web_framework_flutter_api/web_framework.dart';
import 'package:web_framework_flutter_api/web_framework_exception.dart';
import 'package:web_framework_flutter_api/web_framework_utilities.dart';

Future<void> main() async {
  if (Platform.isAndroid) {
    await unpackAndroidAssets();
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
  WebFramework? _server;
  bool _isRunning = false;

  Future<void> start() async {
    try {
      Config config = await Config.fromPath("configs/config.json");

      config.overrideBasePath("${config.handler.assetsPath}/executors");

      _server = await WebFramework.fromConfig(config);

      await _server!.start();

      setState(() {
        _isRunning = true;

        _message = "Server is running";
      });
    } on WebFrameworkException catch (e) {
      _message = e.toString();

      e.dispose();
    } on Exception catch (e) {
      _message = e.toString();
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
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text("Plugin example app"),
        ),
        body: Column(children: [
          Center(
            child: Text(_message),
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
}
