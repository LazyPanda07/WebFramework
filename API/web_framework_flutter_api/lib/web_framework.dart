import 'dart:async';
import 'dart:ffi';
import 'dart:isolate';

import 'package:ffi/ffi.dart';
import 'package:web_framework_flutter_api/dll_handler.dart';
import 'package:web_framework_flutter_api/web_framework_exception.dart';
import 'package:web_framework_flutter_api/web_framework_signatures.dart';

import 'config.dart';

/// Web server
class WebFramework {
  final Pointer<Void> _implementation;
  final DllHandler _handler;
  Isolate? _startIsolate;
  Isolate? _stopIsolate;
  final ReceivePort _startPort = ReceivePort();
  final ReceivePort _stopPort = ReceivePort();

  WebFramework._constructor(this._implementation, this._handler);

  /// Get WebFramework version
  ///
  /// String representation of version in format {major}.{minor}.{patch}
  static Future<String> getWebFrameworkVersion() async {
    DllHandler handler = await DllHandler.create();

    return handler.instance.lookupFunction<GetWebFrameworkVersion, GetWebFrameworkVersion>("getWebFrameworkVersion")().toDartString();
  }

  /// Create WebFramework
  ///
  /// [configPath] Path to *.json config
  static Future<WebFramework> fromPath(String configPath) async {
    DllHandler handler = await DllHandler.create();

    configPath = "${handler.assetsPath}/$configPath";

    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();
    Pointer<Utf8> data = configPath.toNativeUtf8();
    CreateWebFrameworkFromPath function = handler.instance
        .lookupFunction<CreateWebFrameworkFromPath, CreateWebFrameworkFromPath>(
            "createWebFrameworkFromPath");

    Pointer<Void> implementation = function.call(data, exception);

    malloc.free(data);

    WebFrameworkException.checkException(exception, handler);

    return WebFramework._constructor(implementation, handler);
  }

  /// Create WebFramework
  ///
  /// [serverConfiguration] *.json config file content
  ///
  /// [applicationDirectory] Working directory
  static Future<WebFramework> fromString(
      String serverConfiguration, String applicationDirectory) async {
    DllHandler handler = await DllHandler.create();

    applicationDirectory = "${handler.assetsPath}/$applicationDirectory";

    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();
    Pointer<Utf8> serverConfigurationData = serverConfiguration.toNativeUtf8();
    Pointer<Utf8> applicationDirectoryData =
        applicationDirectory.toNativeUtf8();
    CreateWebFrameworkFromString function = handler.instance.lookupFunction<
        CreateWebFrameworkFromString,
        CreateWebFrameworkFromString>("createWebFrameworkFromString");

    Pointer<Void> implementation = function.call(
        serverConfigurationData, applicationDirectoryData, exception);

    malloc.free(serverConfigurationData);
    malloc.free(applicationDirectoryData);

    WebFrameworkException.checkException(exception, handler);

    return WebFramework._constructor(implementation, handler);
  }

  /// Create WebFramework
  ///
  /// [config] Config instance
  static Future<WebFramework> fromConfig(Config config) async {
    DllHandler handler = await DllHandler.create();

    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();

    CreateWebFrameworkFromConfig function = handler.instance.lookupFunction<
        CreateWebFrameworkFromConfig,
        CreateWebFrameworkFromConfig>("createWebFrameworkFromConfig");

    Pointer<Void> implementation =
        function.call(config.implementation, exception);

    WebFrameworkException.checkException(exception, handler);

    return WebFramework._constructor(implementation, handler);
  }

  /// Start server
  Future<void> start({bool wait = false}) async {
    if (_startIsolate != null || _stopIsolate != null) {
      return;
    }

    _startIsolate = await Isolate.spawn((Map data) {
      Pointer<Void> implementation =
          Pointer<Void>.fromAddress(data["implementation"] as int);
      DynamicLibrary library =
          DynamicLibrary.open(data["libraryPath"] as String);
      SendPort port = data["port"] as SendPort;

      Pointer<Pointer<Void>> exception =
          WebFrameworkException.createException();
      StartWebFrameworkServerDart function = library.lookupFunction<
          StartWebFrameworkServerC,
          StartWebFrameworkServerDart>("startWebFrameworkServer");

      function.call(implementation, true, nullptr, exception);

      port.send(exception.address);
    }, {
      "implementation": _implementation.address,
      "libraryPath": _handler.libraryPath,
      "port": _startPort.sendPort
    });

    if (wait) {
      Pointer<Pointer<Void>> exception =
          Pointer<Pointer<Void>>.fromAddress(await _startPort.first as int);

      _startIsolate?.kill(priority: Isolate.immediate);
      _startIsolate = null;

      WebFrameworkException.checkException(exception, _handler);
    } else {
      _startPort.first.then((address) {
        Pointer<Pointer<Void>> exception =
            Pointer<Pointer<Void>>.fromAddress(address as int);

        _startIsolate?.kill(priority: Isolate.immediate);
        _startIsolate = null;

        WebFrameworkException.checkException(exception, _handler);
      });
    }
  }

  /// Stop server
  Future<void> stop({bool wait = true}) async {
    if (_startIsolate == null || _stopIsolate != null) {
      return;
    }

    _stopIsolate = await Isolate.spawn((Map data) {
      Pointer<Void> implementation =
          Pointer<Void>.fromAddress(data["implementation"] as int);
      SendPort port = data["port"] as SendPort;
      DynamicLibrary library =
          DynamicLibrary.open(data["libraryPath"] as String);

      Pointer<Pointer<Void>> exception =
          WebFrameworkException.createException();
      StopWebFrameworkServerDart function = library.lookupFunction<
          StopWebFrameworkServerC,
          StopWebFrameworkServerDart>("stopWebFrameworkServer");

      function.call(implementation, true, exception);

      port.send(exception.address);
    }, {
      "implementation": _implementation.address,
      "libraryPath": _handler.libraryPath,
      "port": _stopPort.sendPort
    });

    if (wait) {
      Pointer<Pointer<Void>> exception =
          Pointer<Pointer<Void>>.fromAddress(await _stopPort.first as int);

      _stopIsolate?.kill(priority: Isolate.immediate);
      _stopIsolate = null;

      WebFrameworkException.checkException(exception, _handler);
    } else {
      _stopPort.first.then((address) {
        Pointer<Pointer<Void>> exception =
            Pointer<Pointer<Void>>.fromAddress(address as int);

        _stopIsolate?.kill(priority: Isolate.immediate);
        _stopIsolate = null;

        WebFrameworkException.checkException(exception, _handler);
      });
    }
  }

  /// Is server running
  bool isServerRunning() {
    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();

    IsServerRunningDart function = _handler.instance
        .lookupFunction<IsServerRunningC, IsServerRunningDart>(
            "isServerRunning");

    bool result = function.call(_implementation, exception);

    WebFrameworkException.checkException(exception, _handler);

    return result;
  }

  void dispose() {
    _handler.deleteWebFramework(_implementation);

    _handler.dispose();
  }
}
