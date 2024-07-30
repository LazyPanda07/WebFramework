import 'package:flutter/material.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:web_framework_flutter_api/config.dart';
import 'package:web_framework_flutter_api/web_framework.dart';
import 'package:web_framework_flutter_api/web_framework_exception.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _message = "";

  @override
  void initState() {
    super.initState();
    initPlatformState();
  }

  Future<void> initPlatformState() async {
    try {
      Config config = await Config.fromPath("configs/multi_threaded_config.json");

      config.overrideBasePath("${config.handler.assetsPath}/executors");

      WebFramework server = await WebFramework.fromConfig(config);

      await server.start(wait: true);
    } on WebFrameworkException catch (e) {
      _message = e.toString();

      e.dispose();
    } on PlatformException catch (e) {
      _message = e.toString();
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;

    setState(() {});
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
          child: Text(_message),
        ),
      ),
    );
  }
}
