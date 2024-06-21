#pragma once

#ifdef WEB_FRAMEWORK_DLL

#include <cstdint>

#ifdef __LINUX__
#define WEB_FRAMEWORK_API extern "C" __attribute__((visibility("default")))
#else
#define WEB_FRAMEWORK_API extern "C" __declspec(dllexport)
#endif

WEB_FRAMEWORK_API void deleteWebFrameworkObject(void* implementation);

WEB_FRAMEWORK_API const char* getDataFromString(void* implementation);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WEB_FRAMEWORK_API void* createWebFrameworkFromPath(const char* configPath, void** exception);

WEB_FRAMEWORK_API void* createWebFrameworkFromString(const char* serverConfiguration, const char* sourcesPath, void** exception);

WEB_FRAMEWORK_API void* createWebFrameworkFromConfig(void* config, void** exception);

WEB_FRAMEWORK_API void* createConfigFromPath(const char* configPath, void** exception);

WEB_FRAMEWORK_API void* createConfigFromString(const char* serverConfiguration, const char* sourcesPath, void** exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WEB_FRAMEWORK_API void startWebFrameworkServerCXX(void* implementation, bool wait, void* onStartServer, void** exception);

WEB_FRAMEWORK_API void startWebFrameworkServer(void* implementation, bool wait, void (*onStartServer)(), void** exception);

WEB_FRAMEWORK_API void stopWebFrameworkServer(void* implementation, bool wait, void** exception);

WEB_FRAMEWORK_API void overrideConfigurationString(void* implementation, const char* key, const char* value, bool recursive, void** exception);

WEB_FRAMEWORK_API void overrideConfigurationInteger(void* implementation, const char* key, int64_t value, bool recursive, void** exception);

WEB_FRAMEWORK_API void overrideConfigurationBoolean(void* implementation, const char* key, bool value, bool recursive, void** exception);

WEB_FRAMEWORK_API void overrideConfigurationStringArray(void* implementation, const char* key, const char** value, bool recursive, int64_t size, void** exception);

WEB_FRAMEWORK_API void overrideConfigurationIntegerArray(void* implementation, const char* key, int64_t* value, bool recursive, int64_t size, void** exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WEB_FRAMEWORK_API const char* getErrorMessage(void* exception);

WEB_FRAMEWORK_API const char* getRawConfiguration(void* implementation, void** exception);

WEB_FRAMEWORK_API void* getConfigurationString(void* implementation, void** exception);

#endif
