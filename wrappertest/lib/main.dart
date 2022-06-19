import 'dart:ffi';
import 'dart:io';
import 'dart:nativewrappers';

void main(List<String> arguments) {
  final dataStore = DataStore();
  dataStore.setData(42);
  print('data is: ${dataStore.getData()}');

  final dataStore2 = DataStore();
  dataStore2.setData(56);
  print('data2 is: ${dataStore2.getData()}');
}

class DataStore extends NativeFieldWrapperClass1 {
  factory DataStore() {
    late final DynamicLibrary lib;
    if (Platform.isWindows) {
      lib = DynamicLibrary.open('native/test.dll');
    } else if (Platform.isMacOS) {
      lib = DynamicLibrary.open('native/test.dylib');
    } else {
      throw UnsupportedError('Platform not supported: ${Platform.operatingSystem}');
    }
    lib.lookupFunction<Void Function(), void Function()>('Library_Init')();
    return _initNative();
  }

  @pragma('vm:external-name', 'DataStore_InitNative')
  external static DataStore _initNative();

  @pragma('vm:external-name', 'DataStore_SetData')
  external void setData(int data);

  @pragma('vm:external-name', 'DataStore_GetData')
  external int getData();
}
