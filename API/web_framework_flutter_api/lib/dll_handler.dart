import 'dart:ffi';

import 'package:ffi/ffi.dart';
import 'package:web_framework_flutter_api/web_framework_flutter_api_platform_interface.dart';

typedef DeleteWebFrameworkC = Void Function(Pointer<Void> implementation);
typedef DeleteWebFrameworkDart = void Function(Pointer<Void> implementation);
typedef GetDataFromString = Pointer<Utf8> Function(Pointer<Void> implementation);

class DllHandler {
  final DynamicLibrary _library;
  final String _assetsPath;

  DllHandler._constructor(this._library, this._assetsPath);

  static Future<DllHandler> create() async {
    String? libraryPath = await WebFrameworkFlutterApiPlatform.instance.getLibraryPath();
    String? assetsPath = await WebFrameworkFlutterApiPlatform.instance.getAssetsPath();

    if (libraryPath == null) {
      throw Exception("Can't get library path");
    }

    if (assetsPath == null) {
      throw Exception("Can't get assets path");
    }

    return DllHandler._constructor(DynamicLibrary.open(libraryPath), assetsPath);
  }

  DynamicLibrary get instance => _library;

  String get assetsPath => _assetsPath;

  void deleteWebFrameworkObject(Pointer<Void> implementation) {
    instance
        .lookupFunction<DeleteWebFrameworkC, DeleteWebFrameworkDart>("deleteWebFrameworkObject")
        .call(implementation);
  }

  String getDataFromString(Pointer<Void> implementation) {
    return instance
        .lookupFunction<GetDataFromString, GetDataFromString>("getDataFromString")
        .call(implementation)
        .toDartString();
  }

  void dispose() {
    _library.close();
  }
}