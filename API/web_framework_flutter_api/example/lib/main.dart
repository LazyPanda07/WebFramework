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
  WidgetsFlutterBinding.ensureInitialized();

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
  String _address = "";
  WebFramework? _server;
  bool _isRunning = false;
  TapGestureRecognizer? _recognizer;

  void _initAddress(Config config) {
    String ip = config.getConfigurationString("ip");
    int port = config.getConfigurationInteger("port");
    bool useHTTPS = config.getConfigurationBoolean("useHTTPS");

    if (ip == "0.0.0.0") {
      ip = "127.0.0.1";
    }

    _address = "${useHTTPS ? "https" : "http"}://$ip:$port";
  }

  Future<void> start() async {
    Config? config;

    try {
      config = await Config.fromPath("configs/config.json");

      config.overrideBasePath("${config.handler.assetsPath}/executors");

      _server = await WebFramework.fromConfig(config);

      await _server!.start();

      setState(() {
        _isRunning = true;

        _initAddress(config!);

        config.dispose();

        _message = "Server is running at ";
      });
    } on WebFrameworkException catch (e) {
      _message = e.toString();

      e.dispose();
      config?.dispose();
    } on Exception catch (e) {
      _message = e.toString();

      config?.dispose();
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
      _recognizer = TapGestureRecognizer()
        ..onTap = () => launchUrl(Uri.parse(_address));
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
              TextSpan(
                  text: _message, style: const TextStyle(color: Colors.black)),
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
                    style: const ButtonStyle(
                        backgroundColor: WidgetStatePropertyAll(Colors.blue),
                        foregroundColor: WidgetStatePropertyAll(Colors.white)),
                    child: const Text("Stop server")))
          else
            Center(
                child: TextButton(
                    onPressed: start,
                    style: const ButtonStyle(
                        backgroundColor: WidgetStatePropertyAll(Colors.blue),
                        foregroundColor: WidgetStatePropertyAll(Colors.white)),
                    child: const Text("Start server")))
        ]),
      ),
    );
  }

  @override
  void dispose() {
    super.dispose();

    _server?.dispose();
    _recognizer?.dispose();
  }
}
