import 'dart:ffi';

import 'package:ffi/ffi.dart';
import 'package:web_framework_flutter_api/web_framework_flutter_api_platform_interface.dart';

typedef DeleteWebFrameworkStringC = Void Function(Pointer<Void> implementation);
typedef DeleteWebFrameworkStringDart = void Function(Pointer<Void> implementation);
typedef DeleteWebFrameworkConfigC = Void Function(Pointer<Void> implementation);
typedef DeleteWebFrameworkConfigDart = void Function(Pointer<Void> implementation);
typedef DeleteWebFrameworkC = Void Function(Pointer<Void> implementation);
typedef DeleteWebFrameworkDart = void Function(Pointer<Void> implementation);
typedef DeleteWebFrameworkExceptionC = Void Function(Pointer<Void> implementation);
typedef DeleteWebFrameworkExceptionDart = void Function(Pointer<Void> implementation);
typedef GetDataFromString = Pointer<Utf8> Function(Pointer<Void> implementation);

class DllHandler {
  final DynamicLibrary _library;
  final String _libraryPath;
  final String _assetsPath;

  DllHandler._constructor(this._library, this._libraryPath, this._assetsPath);

  static Future<DllHandler> create() async {
    String? libraryPath = await WebFrameworkFlutterApiPlatform.instance.getLibraryPath();
    String? assetsPath = await WebFrameworkFlutterApiPlatform.instance.getAssetsPath();

    if (libraryPath == null) {
      throw Exception("Can't get library path");
    }

    if (assetsPath == null) {
      throw Exception("Can't get assets path");
    }

    return DllHandler._constructor(DynamicLibrary.open(libraryPath), libraryPath, assetsPath);
  }

  DynamicLibrary get instance => _library;

  String get libraryPath => _libraryPath;

  String get assetsPath => _assetsPath;

  void deleteWebFrameworkString(Pointer<Void> implementation) {
    instance.lookupFunction<DeleteWebFrameworkStringC, DeleteWebFrameworkStringDart>("deleteWebFrameworkString").call(implementation);
  }

  void deleteWebFrameworkConfig(Pointer<Void> implementation) {
    instance.lookupFunction<DeleteWebFrameworkConfigC, DeleteWebFrameworkConfigDart>("deleteWebFrameworkConfig").call(implementation);
  }

  void deleteWebFramework(Pointer<Void> implementation) {
    instance.lookupFunction<DeleteWebFrameworkC, DeleteWebFrameworkDart>("deleteWebFramework").call(implementation);
  }

  void deleteWebFrameworkException(Pointer<Void> implementation) {
    instance.lookupFunction<DeleteWebFrameworkExceptionC, DeleteWebFrameworkExceptionDart>("deleteWebFrameworkException").call(implementation);
  }

  String getDataFromString(Pointer<Void> implementation) {
    return instance.lookupFunction<GetDataFromString, GetDataFromString>("getDataFromString").call(implementation).toDartString();
  }

  void dispose() {
    _library.close();
  }
}
