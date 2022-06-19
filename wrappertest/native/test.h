#include <windows.h>
#include <stdio.h>
#include <dart_api.h>

#define EXPORT extern "C" __declspec(dllexport)


EXPORT void Library_Init(/* const char *parentLibrary */);
void displayLibs();

Dart_NativeFunction ResolveName(Dart_Handle name, int num_of_arguments, bool* auto_setup_scope);

Dart_Handle HandleError(Dart_Handle handle);

EXPORT void DataStore_InitNative(Dart_NativeArguments arguments);
void DataStore_Dispose(void *isolate_callback_data, void *peer);
EXPORT void DataStore_SetData(Dart_NativeArguments arguments);
EXPORT void DataStore_GetData(Dart_NativeArguments arguments);

