import 'dart:ffi';

import 'package:ffi/ffi.dart';

import 'dll_handler.dart';

typedef GetErrorMessage = Pointer<Utf8> Function(Pointer<Void> implementation);

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
