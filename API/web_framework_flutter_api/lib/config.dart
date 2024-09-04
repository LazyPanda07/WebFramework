import 'dart:ffi';

import 'package:web_framework_flutter_api/web_framework_exception.dart';

import 'dll_handler.dart';

import 'package:ffi/ffi.dart';

typedef CreateConfigFromPath = Pointer<Void> Function(Pointer<Utf8> configPath, Pointer<Pointer<Void>> exception);
typedef CreateConfigFromString = Pointer<Void> Function(
    Pointer<Utf8> serverConfiguration, Pointer<Utf8> applicationDirectory, Pointer<Pointer<Void>> exception);

typedef OverrideConfigurationStringC = Void Function(Pointer<Void> implementation, Pointer<Utf8> key,
    Pointer<Utf8> value, Bool recursive, Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationStringDart = void Function(Pointer<Void> implementation, Pointer<Utf8> key,
    Pointer<Utf8> value, bool recursive, Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationIntegerC = Void Function(
    Pointer<Void> implementation, Pointer<Utf8> key, Int64 value, Bool recursive, Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationIntegerDart = void Function(
    Pointer<Void> implementation, Pointer<Utf8> key, int value, bool recursive, Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationBooleanC = Void Function(
    Pointer<Void> implementation, Pointer<Utf8> key, Bool value, Bool recursive, Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationBooleanDart = void Function(
    Pointer<Void> implementation, Pointer<Utf8> key, bool value, bool recursive, Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationStringArrayC = Void Function(Pointer<Void> implementation, Pointer<Utf8> key,
    Pointer<Pointer<Utf8>> value, Bool recursive, Int64 size, Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationStringArrayDart = void Function(Pointer<Void> implementation, Pointer<Utf8> key,
    Pointer<Pointer<Utf8>> value, bool recursive, int size, Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationIntegerArrayC = Void Function(Pointer<Void> implementation, Pointer<Utf8> key,
    Pointer<Int64> value, Bool recursive, Int64 size, Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationIntegerArrayDart = void Function(Pointer<Void> implementation, Pointer<Utf8> key,
    Pointer<Int64> value, bool recursive, int size, Pointer<Pointer<Void>> exception);
typedef OverrideBasePathC = Void Function(
    Pointer<Void> implementation, Pointer<Utf8> basePath, Pointer<Pointer<Void>> exception);
typedef OverrideBasePathDart = void Function(
    Pointer<Void> implementation, Pointer<Utf8> basePath, Pointer<Pointer<Void>> exception);

typedef GetBasePath = Pointer<Void> Function(Pointer<Void> implementation, Pointer<Pointer<Void>>);
typedef GetConfiguration = Pointer<Void> Function(Pointer<Void> implementation, Pointer<Pointer<Void>> exception);
typedef GetRawConfiguration = Pointer<Utf8> Function(Pointer<Void> implementation, Pointer<Pointer<Void>> exception);

/// Config file representation
class Config {
  final Pointer<Void> implementation;
  final DllHandler handler;

  Config._constructor(this.implementation, this.handler);

  /// Create config
  ///
  /// [configPath] Path to *.json config file
  static Future<Config> fromPath(String configPath) async {
    DllHandler handler = await DllHandler.create();

    configPath = "${handler.assetsPath}/$configPath";

    Pointer<Utf8> data = configPath.toNativeUtf8();
    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();

    CreateConfigFromPath function =
        handler.instance.lookupFunction<CreateConfigFromPath, CreateConfigFromPath>("createConfigFromPath");

    Pointer<Void> implementation = function.call(data, exception);

    malloc.free(data);

    WebFrameworkException.checkException(exception, handler);

    return Config._constructor(implementation, handler);
  }

  /// Create config
  ///
  /// [serverConfiguration] *.json config file content
  ///
  /// [applicationDirectory] Working directory
  static Future<Config> fromString(String serverConfiguration, String applicationDirectory) async {
    DllHandler handler = await DllHandler.create();

    applicationDirectory = "${handler.assetsPath}/$applicationDirectory";

    Pointer<Utf8> serverConfigurationData = serverConfiguration.toNativeUtf8();
    Pointer<Utf8> applicationDirectoryData = applicationDirectory.toNativeUtf8();
    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();

    CreateConfigFromString function =
        handler.instance.lookupFunction<CreateConfigFromString, CreateConfigFromString>("createConfigFromString");

    Pointer<Void> implementation = function.call(serverConfigurationData, applicationDirectoryData, exception);

    malloc.free(serverConfigurationData);
    malloc.free(applicationDirectoryData);

    WebFrameworkException.checkException(exception, handler);

    return Config._constructor(implementation, handler);
  }

  /// Override string
  ///
  /// [key] JSON key
  ///
  /// [value] New string value
  ///
  /// [recursive] Recursive search for key
  void overrideConfigurationString(String key, String value, {bool recursive = false}) {
    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();
    Pointer<Utf8> keyData = key.toNativeUtf8();
    Pointer<Utf8> valueData = value.toNativeUtf8();

    OverrideConfigurationStringDart function = handler.instance
        .lookupFunction<OverrideConfigurationStringC, OverrideConfigurationStringDart>("overrideConfigurationString");

    function.call(implementation, keyData, valueData, recursive, exception);

    malloc.free(keyData);
    malloc.free(valueData);

    WebFrameworkException.checkException(exception, handler);
  }

  /// Override integer
  ///
  /// [key] JSON key
  ///
  /// [value] New integer value
  ///
  /// [recursive] Recursive search for key
  void overrideConfigurationInteger(String key, int value, {bool recursive = false}) {
    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();
    Pointer<Utf8> keyData = key.toNativeUtf8();

    OverrideConfigurationIntegerDart function = handler.instance
        .lookupFunction<OverrideConfigurationIntegerC, OverrideConfigurationIntegerDart>(
            "overrideConfigurationInteger");

    function.call(implementation, keyData, value, recursive, exception);

    malloc.free(keyData);

    WebFrameworkException.checkException(exception, handler);
  }

  /// Override bool
  ///
  /// [key] JSON key
  ///
  /// [value] New bool value
  ///
  /// [recursive] Recursive search for key
  void overrideConfigurationBoolean(String key, bool value, {bool recursive = false}) {
    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();
    Pointer<Utf8> keyData = key.toNativeUtf8();

    OverrideConfigurationBooleanDart function = handler.instance
        .lookupFunction<OverrideConfigurationBooleanC, OverrideConfigurationBooleanDart>(
            "overrideConfigurationBoolean");

    function.call(implementation, keyData, value, recursive, exception);

    malloc.free(keyData);

    WebFrameworkException.checkException(exception, handler);
  }

  /// Override string array
  ///
  /// [key] JSON key
  ///
  /// [value] New string array value
  ///
  /// [recursive] Recursive search for key
  void overrideConfigurationStringArray(String key, List<String> value, {bool recursive = false}) {
    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();
    Pointer<Utf8> keyData = key.toNativeUtf8();
    Pointer<Pointer<Utf8>> valueData = malloc.allocate(sizeOf<Pointer<Utf8>>() * value.length);

    for (int i = 0; i < value.length; i++) {
      valueData[i] = value[i].toNativeUtf8();
    }

    OverrideConfigurationStringArrayDart function = handler.instance
        .lookupFunction<OverrideConfigurationStringArrayC, OverrideConfigurationStringArrayDart>(
            "overrideConfigurationStringArray");

    function(implementation, keyData, valueData, recursive, value.length, exception);

    malloc.free(keyData);

    for (int i = 0; i < value.length; i++) {
      malloc.free(valueData[i]);
    }

    malloc.free(valueData);

    WebFrameworkException.checkException(exception, handler);
  }

  /// Override integer array
  ///
  /// [key] JSON key
  ///
  /// [value] New integer array value
  ///
  /// [recursive] Recursive search for key
  void overrideConfigurationIntegerArray(String key, List<int> value, {bool recursive = false}) {
    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();
    Pointer<Utf8> keyData = key.toNativeUtf8();
    Pointer<Int64> valueData = malloc.allocate(sizeOf<Int64>() * value.length);

    for (int i = 0; i < value.length; i++) {
      valueData[i] = value[i];
    }

    OverrideConfigurationIntegerArrayDart function = handler.instance
        .lookupFunction<OverrideConfigurationIntegerArrayC, OverrideConfigurationIntegerArrayDart>(
            "overrideConfigurationIntegerArray");

    function(implementation, keyData, valueData, recursive, value.length, exception);

    malloc.free(keyData);
    malloc.free(valueData);

    WebFrameworkException.checkException(exception, handler);
  }

  /// Override config file directory
  ///
  /// [basePath] New base path
  void overrideBasePath(String basePath) {
    Pointer<Utf8> data = basePath.toNativeUtf8();
    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();

    OverrideBasePathDart function =
        handler.instance.lookupFunction<OverrideBasePathC, OverrideBasePathDart>("overrideBasePath");

    function.call(implementation, data, exception);

    malloc.free(data);

    WebFrameworkException.checkException(exception, handler);
  }

  /// Get current config JSON string data
  String getConfiguration() {
    GetConfiguration function = handler.instance.lookupFunction<GetConfiguration, GetConfiguration>("getConfiguration");

    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();
    Pointer<Void> temp = function.call(implementation, exception);

    WebFrameworkException.checkException(exception, handler);

    String result = handler.getDataFromString(temp);

    handler.deleteWebFrameworkObject(temp);

    return result;
  }

  /// Get raw config JSON string data
  String getRawConfiguration() {
    GetRawConfiguration function =
        handler.instance.lookupFunction<GetRawConfiguration, GetRawConfiguration>("getRawConfiguration");

    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();
    Pointer<Utf8> result = function.call(implementation, exception);

    WebFrameworkException.checkException(exception, handler);

    return result.toDartString();
  }

  /// Get config file directory
  String getBasePath() {
    Pointer<Pointer<Void>> exception = WebFrameworkException.createException();

    GetBasePath function = handler.instance.lookupFunction<GetBasePath, GetBasePath>("getBasePath");

    Pointer<Void> string = function.call(implementation, exception);

    WebFrameworkException.checkException(exception, handler);

    String result = handler.getDataFromString(string);

    handler.deleteWebFrameworkObject(string);

    return result;
  }

  void dispose() {
    handler.deleteWebFrameworkObject(implementation);

    handler.dispose();
  }
}
