#pragma once

#ifdef WEB_FRAMEWORK_DLL

#include <stdint.h>

#ifdef __LINUX__
#define EXPORT extern "C" __attribute__((visibility("default")))
#else
#define EXPORT extern "C" __declspec(dllexport)
#endif

typedef void* String;
typedef void* Config;
typedef void* WebFramework;
typedef void* Exception;

EXPORT void deleteWebFrameworkObject(void* implementation);

EXPORT const char* getDataFromString(String string);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT WebFramework createWebFrameworkFromPath(const char* configPath, Exception* exception);

EXPORT WebFramework createWebFrameworkFromString(const char* serverConfiguration, const char* applicationDirectory, Exception* exception);

EXPORT WebFramework createWebFrameworkFromConfig(Config config, Exception* exception);

EXPORT Config createConfigFromPath(const char* configPath, Exception* exception);

EXPORT Config createConfigFromString(const char* serverConfiguration, const char* applicationDirectory, Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT void startWebFrameworkServerCXX(WebFramework server, bool wait, void* onStartServer, Exception* exception);

EXPORT void startWebFrameworkServer(WebFramework server, bool wait, void (*onStartServer)(), Exception* exception);

EXPORT void stopWebFrameworkServer(WebFramework server, bool wait, Exception* exception);

EXPORT bool isServerRunning(WebFramework server, Exception* exception);

EXPORT void overrideConfigurationString(Config config, const char* key, const char* value, bool recursive, Exception* exception);

EXPORT void overrideConfigurationInteger(Config config, const char* key, int64_t value, bool recursive, Exception* exception);

EXPORT void overrideConfigurationBoolean(Config config, const char* key, bool value, bool recursive, Exception* exception);

EXPORT void overrideConfigurationStringArray(Config config, const char* key, const char** value, bool recursive, int64_t size, Exception* exception);

EXPORT void overrideConfigurationIntegerArray(Config config, const char* key, const int64_t* value, bool recursive, int64_t size, Exception* exception);

EXPORT void overrideBasePath(Config config, const char* basePath, Exception* exception);

EXPORT String getConfigurationString(Config config, const char* key, bool recursive, Exception* exception);

EXPORT int64_t getConfigurationInteger(Config config, const char* key, bool recursive, Exception* exception);

EXPORT bool getConfigurationBoolean(Config config, const char* key, bool recursive, Exception* exception);

EXPORT void* getConfiguration(Config config, Exception* exception);

EXPORT const char* getRawConfiguration(Config config, Exception* exception);

EXPORT void* getBasePath(Config config, Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT const char* getErrorMessage(Exception exception);

#endif
