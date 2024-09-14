import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'web_framework_flutter_api_platform_interface.dart';

/// An implementation of [WebFrameworkFlutterApiPlatform] that uses method channels.
class MethodChannelWebFrameworkFlutterApi extends WebFrameworkFlutterApiPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('web_framework_flutter_api');

  @override
  Future<String?> getLibraryPath() async {
    return await methodChannel.invokeMethod("getLibraryPath");
  }

  @override
  Future<String?> getAssetsPath() async {
    return await methodChannel.invokeMethod("getAssetsPath");
  }
}
