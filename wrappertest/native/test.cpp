#include "test.h"

static bool initialized = false;
static Dart_Handle rootLibrary;
static Dart_Handle dataStoreType;

typedef struct {
	int64_t value;
} DataStoreData;

struct FunctionLookup {
	const char* name;
	Dart_NativeFunction function;
};

static FunctionLookup resolveFnList[] = {
	{"DataStore_InitNative", DataStore_InitNative},
	{"DataStore_SetData",    DataStore_SetData},
	{"DataStore_GetData",    DataStore_GetData},
	{NULL, NULL},
};


void Library_Init(/* const char *parentLibrary */) {
	if(initialized) {
		return;
	}
	initialized = true;

	Dart_EnterScope();

	// printf("Library_Init\n");
	// displayLibs();

	// rootLibrary = HandleError(Dart_NewPersistentHandle(HandleError(
	// 	Dart_LookupLibrary(Dart_NewStringFromCString(parentLibrary))
	// )));

	rootLibrary = Dart_RootLibrary();

	dataStoreType = HandleError(Dart_GetNonNullableType(
		rootLibrary, Dart_NewStringFromCString("DataStore"), 0, NULL)
	);

	HandleError(Dart_SetNativeResolver(rootLibrary, ResolveName, NULL));

	Dart_ExitScope();
}

const char *toString(Dart_Handle handle) {
	const char *result = NULL;
	HandleError(Dart_StringToCString(HandleError(Dart_ToString(handle)), &result));
	return result;
}

void displayLibs() {
	Dart_EnterScope();
	Dart_Handle libs = HandleError(Dart_GetLoadedLibraries());
	intptr_t length = 0;
	Dart_ListLength(libs, &length);
	printf("count: %d\n", (int)length);
	for(int i = 0; i < length; i++) {
		Dart_Handle lib = Dart_ListGetAt(libs, i);
		printf("item %d: %s\n", i, toString(lib));
	}
	Dart_ExitScope();
}

Dart_NativeFunction ResolveName(Dart_Handle name, int num_of_arguments, bool* auto_setup_scope)
{
	Dart_EnterScope();

	const char *cname;
	HandleError(Dart_StringToCString(name, &cname));

	Dart_NativeFunction result = NULL;
	for (int i = 0; resolveFnList[i].name != NULL; i++) {
		if (strcmp(resolveFnList[i].name, cname) == 0) {
			*auto_setup_scope = true;
			result = resolveFnList[i].function;
			break;
		}
	}

	Dart_ExitScope();
	return result;
}

Dart_Handle HandleError(Dart_Handle handle)
{
	if (Dart_IsError(handle)) {
		Dart_PropagateError(handle);
	}
	return handle;
}

EXPORT void DataStore_InitNative(Dart_NativeArguments arguments)
{
	DataStoreData *data = new DataStoreData();
	data->value = 0;
	Dart_Handle dataStore = HandleError(
		Dart_AllocateWithNativeFields(dataStoreType, 1, reinterpret_cast<intptr_t *>(&data))
	);
	Dart_WeakPersistentHandle result = Dart_NewWeakPersistentHandle(
		dataStore, reinterpret_cast<void *>(data), sizeof(DataStoreData), DataStore_Dispose);
	Dart_SetWeakHandleReturnValue(arguments, result);
}

void DataStore_Dispose(void *isolate_callback_data, void *peer) {
	DataStoreData *data = reinterpret_cast<DataStoreData *>(peer);
	delete data;
}

EXPORT void DataStore_SetData(Dart_NativeArguments arguments)
{
	DataStoreData *data = nullptr;
	Dart_Handle dataStore = HandleError(Dart_GetNativeArgument(arguments, 0));
	HandleError(Dart_GetNativeInstanceField(dataStore, 0, reinterpret_cast<intptr_t *>(&data)));
	int64_t value = 0;
	HandleError(Dart_GetNativeIntegerArgument(arguments, 1, &value));
	data->value = value;
}

EXPORT void DataStore_GetData(Dart_NativeArguments arguments)
{
	DataStoreData *data = nullptr;
	Dart_Handle dataStore = HandleError(Dart_GetNativeArgument(arguments, 0));
	HandleError(Dart_GetNativeInstanceField(dataStore, 0, reinterpret_cast<intptr_t *>(&data)));
	Dart_SetIntegerReturnValue(arguments, data->value);
}
