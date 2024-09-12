import 'dart:async';
import 'dart:ffi';
import 'dart:isolate';

import 'package:ffi/ffi.dart';
import 'package:web_framework_flutter_api/dll_handler.dart';
import 'package:web_framework_flutter_api/web_framework_exception.dart';

import 'config.dart';

typedef OnStartServerC = Void Function();
typedef OnStartServerDart = void Function();
typedef CreateWebFrameworkFromConfig = Pointer<Void> Function(Pointer<Void> config, Pointer<Pointer<Void>> exception);
typedef CreateWebFrameworkFromString = Pointer<Void> Function(
    Pointer<Utf8> serverConfiguration, Pointer<Utf8> applicationDirectory, Pointer<Pointer<Void>> exception);
typedef CreateWebFrameworkFromPath = Pointer<Void> Function(Pointer<Utf8> configPath, Pointer<Pointer<Void>> exception);
typedef StartWebFrameworkServerC = Void Function(
    Pointer<Void> implementation, Bool wait, Pointer<NativeFunction<OnStartServerC>> callback, Pointer<Pointer<Void>> exception);
typedef StartWebFrameworkServerDart = void Function(
    Pointer<Void> implementation, bool wait, Pointer<NativeFunction<OnStartServerC>> callback, Pointer<Pointer<Void>> exception);
typedef StopWebFrameworkServerC = Void Function(Pointer<Void> implementation, Pointer<Pointer<Void>> exception);
typedef StopWebFrameworkServerDart = void Function(Pointer<Void> implementation, Pointer<Pointer<Void>> exception);

/// Web server
class WebFramework {
  final Pointer<Void> _implementation;
  final DllHandler _handler;

  WebFramework._constructor(this._implementation, this._handler);

  /// Create WebFramework
  ///
  /// [configPath] Path to *.json config
  static Future<WebFramework> fromPath(String configPath) async {
    DllHandler handler = await DllHandler.create();

    configPath = "${handler.assetsPath}/$configPath";

    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();
    Pointer<Utf8> data = configPath.toNativeUtf8();
    CreateWebFrameworkFromPath function = handler.instance.lookupFunction<CreateWebFrameworkFromPath, CreateWebFrameworkFromPath>("createWebFrameworkFromPath");

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
  static Future<WebFramework> fromString(String serverConfiguration, String applicationDirectory) async {
    DllHandler handler = await DllHandler.create();

    applicationDirectory = "${handler.assetsPath}/$applicationDirectory";

    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();
    Pointer<Utf8> serverConfigurationData = serverConfiguration.toNativeUtf8();
    Pointer<Utf8> applicationDirectoryData = applicationDirectory.toNativeUtf8();
    CreateWebFrameworkFromString function =
        handler.instance.lookupFunction<CreateWebFrameworkFromString, CreateWebFrameworkFromString>("createWebFrameworkFromString");

    Pointer<Void> implementation = function.call(serverConfigurationData, applicationDirectoryData, exception);

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

    CreateWebFrameworkFromConfig function =
        handler.instance.lookupFunction<CreateWebFrameworkFromConfig, CreateWebFrameworkFromConfig>("createWebFrameworkFromConfig");

    Pointer<Void> implementation = function.call(config.implementation, exception);

    WebFrameworkException.checkException(exception, handler);

    return WebFramework._constructor(implementation, handler);
  }

  /// Start server
  ///
  /// [wait] Wait until server stop
  Future<void> start({bool wait = false}) async {
    Isolate.spawn((Map data) {
      Pointer<Pointer<Void>> exception = WebFrameworkException.createException();
      Pointer<Void> implementation = Pointer<Void>.fromAddress(data["implementation"] as int);
      DynamicLibrary library = DynamicLibrary.open(data["libraryPath"] as String);
      StartWebFrameworkServerDart function = library.lookupFunction<StartWebFrameworkServerC, StartWebFrameworkServerDart>("startWebFrameworkServer");

      function.call(implementation, true, nullptr, exception);

      // WebFrameworkException.checkException(exception, _handler);
    }, {"implementation": _implementation.address, "libraryPath": _handler.libraryPath});
  }

  /// Stop server
  ///
  /// [wait] Wait until server stop
  Future<void> stop({bool wait = false}) async {
    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();

    StopWebFrameworkServerDart function = _handler.instance.lookupFunction<StopWebFrameworkServerC, StopWebFrameworkServerDart>("stopWebFrameworkServer");

    function.call(_implementation, exception);

    WebFrameworkException.checkException(exception, _handler);
  }

  void dispose() {
    _handler.deleteWebFrameworkObject(_implementation);

    _handler.dispose();
  }
}
