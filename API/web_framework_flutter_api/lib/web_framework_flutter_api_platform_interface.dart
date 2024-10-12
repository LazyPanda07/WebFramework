import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'web_framework_flutter_api_method_channel.dart';

abstract class WebFrameworkFlutterApiPlatform extends PlatformInterface {
  /// Constructs a WebFrameworkFlutterApiPlatform.
  WebFrameworkFlutterApiPlatform() : super(token: _token);

  static final Object _token = Object();

  static WebFrameworkFlutterApiPlatform _instance =
      MethodChannelWebFrameworkFlutterApi();

  /// The default instance of [WebFrameworkFlutterApiPlatform] to use.
  ///
  /// Defaults to [MethodChannelWebFrameworkFlutterApi].
  static WebFrameworkFlutterApiPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [WebFrameworkFlutterApiPlatform] when
  /// they register themselves.
  static set instance(WebFrameworkFlutterApiPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getLibraryPath() async => throw UnimplementedError();

  Future<String?> getAssetsPath() async => throw UnimplementedError();
}
