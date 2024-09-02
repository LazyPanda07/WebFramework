import 'dart:convert';
import 'dart:io';

import 'package:flutter/services.dart';
import 'package:path_provider/path_provider.dart';
import 'package:crypto/crypto.dart';

Future<String> unpackAndroidAssets() async {
  String manifest = await rootBundle.loadString("AssetManifest.json");
  Map<String, dynamic> manifestContent = jsonDecode(manifest);
  Directory documentsDirectory = await getApplicationDocumentsDirectory();

  for (String key in manifestContent.keys) {
    String filePath = "${documentsDirectory.path}/$key";
    ByteData data = await rootBundle.load(key);
    List<int> bytes = data.buffer.asUint8List(data.offsetInBytes, data.lengthInBytes);
    File file = File(filePath);
    FileSystemEntityType type = FileSystemEntity.typeSync(filePath);

    if (type == FileSystemEntityType.notFound) {
      file.createSync(recursive: true);

      await file.writeAsBytes(bytes);
    } else if (type == FileSystemEntityType.file) {
      if (md5.convert(bytes) != md5.convert(file.readAsBytesSync())) {
        file.deleteSync();
        file.createSync();

        await file.writeAsBytes(bytes);
      }
    }
  }

  return "${documentsDirectory.path}/assets";
}
