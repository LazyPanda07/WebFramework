import 'dart:ffi';

import 'package:ffi/ffi.dart';

typedef CreateConfigFromPath = Pointer<Void> Function(
    Pointer<Utf8> configPath, Pointer<Pointer<Void>> exception);
typedef CreateConfigFromString = Pointer<Void> Function(
    Pointer<Utf8> serverConfiguration,
    Pointer<Utf8> applicationDirectory,
    Pointer<Pointer<Void>> exception);

typedef OverrideConfigurationStringC = Void Function(
    Pointer<Void> implementation,
    Pointer<Utf8> key,
    Pointer<Utf8> value,
    Bool recursive,
    Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationStringDart = void Function(
    Pointer<Void> implementation,
    Pointer<Utf8> key,
    Pointer<Utf8> value,
    bool recursive,
    Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationIntegerC = Void Function(
    Pointer<Void> implementation,
    Pointer<Utf8> key,
    Int64 value,
    Bool recursive,
    Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationIntegerDart = void Function(
    Pointer<Void> implementation,
    Pointer<Utf8> key,
    int value,
    bool recursive,
    Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationBooleanC = Void Function(
    Pointer<Void> implementation,
    Pointer<Utf8> key,
    Bool value,
    Bool recursive,
    Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationBooleanDart = void Function(
    Pointer<Void> implementation,
    Pointer<Utf8> key,
    bool value,
    bool recursive,
    Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationStringArrayC = Void Function(
    Pointer<Void> implementation,
    Pointer<Utf8> key,
    Pointer<Pointer<Utf8>> value,
    Bool recursive,
    Int64 size,
    Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationStringArrayDart = void Function(
    Pointer<Void> implementation,
    Pointer<Utf8> key,
    Pointer<Pointer<Utf8>> value,
    bool recursive,
    int size,
    Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationIntegerArrayC = Void Function(
    Pointer<Void> implementation,
    Pointer<Utf8> key,
    Pointer<Int64> value,
    Bool recursive,
    Int64 size,
    Pointer<Pointer<Void>> exception);
typedef OverrideConfigurationIntegerArrayDart = void Function(
    Pointer<Void> implementation,
    Pointer<Utf8> key,
    Pointer<Int64> value,
    bool recursive,
    int size,
    Pointer<Pointer<Void>> exception);
typedef OverrideBasePathC = Void Function(Pointer<Void> implementation,
    Pointer<Utf8> basePath, Pointer<Pointer<Void>> exception);
typedef OverrideBasePathDart = void Function(Pointer<Void> implementation,
    Pointer<Utf8> basePath, Pointer<Pointer<Void>> exception);

typedef GetConfigurationStringC = Pointer<Void> Function(
    Pointer<Void> implementation,
    Pointer<Utf8> key,
    Bool recursive,
    Pointer<Pointer<Void>> exception);
typedef GetConfigurationStringDart = Pointer<Void> Function(
    Pointer<Void> implementation,
    Pointer<Utf8> key,
    bool recursive,
    Pointer<Pointer<Void>> exception);
typedef GetConfigurationIntegerC = Int64 Function(Pointer<Void> implementation,
    Pointer<Utf8> key, Bool recursive, Pointer<Pointer<Void>> exception);
typedef GetConfigurationIntegerDart = int Function(Pointer<Void> implementation,
    Pointer<Utf8> key, bool recursive, Pointer<Pointer<Void>> exception);
typedef GetConfigurationBooleanC = Bool Function(Pointer<Void> implementation,
    Pointer<Utf8> key, Bool recursive, Pointer<Pointer<Void>> exception);
typedef GetConfigurationBooleanDart = bool Function(
    Pointer<Void> implementation,
    Pointer<Utf8> key,
    bool recursive,
    Pointer<Pointer<Void>> exception);

typedef GetBasePath = Pointer<Void> Function(
    Pointer<Void> implementation, Pointer<Pointer<Void>> exception);
typedef GetConfiguration = Pointer<Void> Function(
    Pointer<Void> implementation, Pointer<Pointer<Void>> exception);
typedef GetRawConfiguration = Pointer<Utf8> Function(
    Pointer<Void> implementation, Pointer<Pointer<Void>> exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef OnStartServerC = Void Function();
typedef OnStartServerDart = void Function();
typedef CreateWebFrameworkFromConfig = Pointer<Void> Function(
    Pointer<Void> config, Pointer<Pointer<Void>> exception);
typedef CreateWebFrameworkFromString = Pointer<Void> Function(
    Pointer<Utf8> serverConfiguration,
    Pointer<Utf8> applicationDirectory,
    Pointer<Pointer<Void>> exception);
typedef CreateWebFrameworkFromPath = Pointer<Void> Function(
    Pointer<Utf8> configPath, Pointer<Pointer<Void>> exception);
typedef StartWebFrameworkServerC = Void Function(
    Pointer<Void> implementation,
    Bool wait,
    Pointer<NativeFunction<OnStartServerC>> callback,
    Pointer<Pointer<Void>> exception);
typedef StartWebFrameworkServerDart = void Function(
    Pointer<Void> implementation,
    bool wait,
    Pointer<NativeFunction<OnStartServerC>> callback,
    Pointer<Pointer<Void>> exception);
typedef StopWebFrameworkServerC = Void Function(
    Pointer<Void> implementation, Bool wait, Pointer<Pointer<Void>> exception);
typedef StopWebFrameworkServerDart = void Function(
    Pointer<Void> implementation, bool wait, Pointer<Pointer<Void>> exception);
typedef IsServerRunningC = Bool Function(
    Pointer<Void> implementation, Pointer<Pointer<Void>> exception);
typedef IsServerRunningDart = bool Function(
    Pointer<Void> implementation, Pointer<Pointer<Void>> exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef GetErrorMessage = Pointer<Utf8> Function(Pointer<Void> implementation);
