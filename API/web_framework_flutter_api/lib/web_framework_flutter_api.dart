
import 'web_framework_flutter_api_platform_interface.dart';

class WebFrameworkFlutterApi {
  Future<String?> getPlatformVersion() {
    return WebFrameworkFlutterApiPlatform.instance.getPlatformVersion();
  }
}
