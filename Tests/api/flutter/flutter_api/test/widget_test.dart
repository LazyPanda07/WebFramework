// This is a basic Flutter widget test.
//
// To perform an interaction with a widget in your test, use the WidgetTester
// utility in the flutter_test package. For example, you can send tap and scroll
// gestures. You can also use WidgetTester to find child widgets in the widget
// tree, read text, and verify that the values of widget properties are correct.

import 'package:flutter/material.dart';
import 'package:flutter_test/flutter_test.dart';

import 'package:flutter_api/main.dart';
import 'package:web_framework_flutter_api/config.dart';
import 'package:web_framework_flutter_api/web_framework.dart';
import 'package:web_framework_flutter_api/web_framework_exception.dart';

void main() {
  WidgetsFlutterBinding.ensureInitialized();

  test("Create Config from path", () async {
    Config config = await Config.fromPath("C:/Users/semen/source/repos/WebFramework/Tests/build/bin/multi_threaded_config.json");


  });
}
