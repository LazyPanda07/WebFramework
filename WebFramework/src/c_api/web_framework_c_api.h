#pragma once

#ifdef WEB_FRAMEWORK_DLL

#include <cstdint>

#ifdef __LINUX__
#define EXPORT extern "C" __attribute__((visibility("default")))
#else
#define EXPORT extern "C" __declspec(dllexport)
#endif

EXPORT void deleteWebFrameworkObject(void* implementation);

EXPORT const char* getDataFromString(void* implementation);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT void* createWebFrameworkFromPath(const char* configPath, void** exception);

EXPORT void* createWebFrameworkFromString(const char* serverConfiguration, const char* applicationDirectory, void** exception);

EXPORT void* createWebFrameworkFromConfig(void* config, void** exception);

EXPORT void* createConfigFromPath(const char* configPath, void** exception);

EXPORT void* createConfigFromString(const char* serverConfiguration, const char* applicationDirectory, void** exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT void startWebFrameworkServerCXX(void* implementation, bool wait, void* onStartServer, void** exception);

EXPORT void startWebFrameworkServer(void* implementation, bool wait, void (*onStartServer)(), void** exception);

EXPORT void stopWebFrameworkServer(void* implementation, bool wait, void** exception);

EXPORT void overrideConfigurationString(void* implementation, const char* key, const char* value, bool recursive, void** exception);

EXPORT void overrideConfigurationInteger(void* implementation, const char* key, int64_t value, bool recursive, void** exception);

EXPORT void overrideConfigurationBoolean(void* implementation, const char* key, bool value, bool recursive, void** exception);

EXPORT void overrideConfigurationStringArray(void* implementation, const char* key, const char** value, bool recursive, int64_t size, void** exception);

EXPORT void overrideConfigurationIntegerArray(void* implementation, const char* key, const int64_t* value, bool recursive, int64_t size, void** exception);

EXPORT void overrideBasePath(void* implementation, const char* basePath, void** exception);

EXPORT void* getConfiguration(void* implementation, void** exception);

EXPORT const char* getRawConfiguration(void* implementation, void** exception);

EXPORT void* getBasePath(void* implementation, void** exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT const char* getErrorMessage(void* exception);

#endif
