import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:web_framework_flutter_api/web_framework_flutter_api_method_channel.dart';

void main() {
  TestWidgetsFlutterBinding.ensureInitialized();

  MethodChannelWebFrameworkFlutterApi platform = MethodChannelWebFrameworkFlutterApi();
  const MethodChannel channel = MethodChannel('web_framework_flutter_api');

  setUp(() {
    TestDefaultBinaryMessengerBinding.instance.defaultBinaryMessenger.setMockMethodCallHandler(
      channel,
      (MethodCall methodCall) async {
        return '42';
      },
    );
  });

  tearDown(() {
    TestDefaultBinaryMessengerBinding.instance.defaultBinaryMessenger.setMockMethodCallHandler(channel, null);
  });

  test('getPlatformVersion', () async {
    expect(await platform.getPlatformVersion(), '42');
  });
}
