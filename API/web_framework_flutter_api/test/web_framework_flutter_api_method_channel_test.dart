import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:web_framework_flutter_api/config.dart';
import 'package:web_framework_flutter_api/web_framework_flutter_api_method_channel.dart';

void main() {
  TestWidgetsFlutterBinding.ensureInitialized();

  test('getPlatformVersion', () async {
    Config config = await Config.fromPath("C:/Users/semen/source/repos/WebFramework/Tests/build/bin/multi_threaded_config.json");
  });
}
