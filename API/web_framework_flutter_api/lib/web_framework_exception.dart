import 'dart:ffi';

import 'package:ffi/ffi.dart';
import 'package:web_framework_flutter_api/web_framework_signatures.dart';

import 'dll_handler.dart';

/// Exception class for WebFramework exceptions
class WebFrameworkException implements Exception {
  final Pointer<Pointer<Void>> _implementation;
  final DllHandler _handler;

  WebFrameworkException(this._implementation, this._handler);

  static Pointer<Pointer<Void>> createException() {
    Pointer<Pointer<Void>> exception = malloc.allocate(sizeOf<Pointer<Void>>());
    exception.value = nullptr;

    return exception;
  }

  static void checkException(Pointer<Pointer<Void>> exception, DllHandler handler) {
    if (exception.value.address != 0) {
      throw WebFrameworkException(exception, handler);
    }
  }

  void dispose() {
    _handler.deleteWebFrameworkObject(_implementation.value);

    malloc.free(_implementation);

    _handler.dispose();
  }

  @override
  String toString() {
    return _handler.instance
        .lookupFunction<GetErrorMessage, GetErrorMessage>("getErrorMessage")
        .call(_implementation.value)
        .toDartString();
  }
}
