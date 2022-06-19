import 'dart:ffi';
import 'dart:io';

import 'package:miniaudio/miniaudio.dart';
import 'package:ffi/ffi.dart';

typedef MaLogCallbackFn = Void Function(Pointer<Void>, ma_uint32, Pointer<Char>);

void _log(Pointer<Void> userData, int logLevel, Pointer<Char> message) {
  print('Log: $logLevel | ${message.cast<Utf8>().toDartString()}');
}

void main(List<String> arguments) {
  late final DynamicLibrary lib;
  if (Platform.isWindows) {
    lib = DynamicLibrary.open('../miniaudio/native/miniaudio.dll');
  } else if (Platform.isMacOS) {
    lib = DynamicLibrary.open('../miniaudio/native/miniaudio.dylib');
  } else {
    throw UnsupportedError('Platform not supported: ${Platform.operatingSystem}');
  }

  final miniAudio = MiniAudio(lib);

  final engine = malloc<ma_engine>();
  final result = miniAudio.ma_engine_init(nullptr, engine);
  if (result != ma_result.MA_SUCCESS) {
    print('Failed to initialize audio engine.');
    return;
  }

  final logCallbackPtr = Pointer.fromFunction<MaLogCallbackFn>(_log);
  final logCallback = miniAudio.ma_log_callback_init(logCallbackPtr, nullptr);
  miniAudio.ma_log_register_callback(engine.ref.pLog, logCallback);

  print('Attempting playback.');
  miniAudio.ma_engine_play_sound(engine, '../StarWars60.wav'.toNativeUtf8().cast(), nullptr);

  miniAudio.ma_log_post(
    engine.ref.pLog,
    ma_log_level.MA_LOG_LEVEL_INFO,
    'Callback testing'.toNativeUtf8().cast(),
  );

  print('Press Enter to quit...');
  stdin.readLineSync();

  miniAudio.ma_engine_uninit(engine);

  malloc.free(engine);
}
