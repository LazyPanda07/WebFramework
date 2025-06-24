#pragma once

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
typedef void* JSONBuilder;
typedef void* JSONObject;

EXPORT void deleteWebFrameworkString(String string);

EXPORT void deleteWebFrameworkConfig(Config config);

EXPORT void deleteWebFramework(WebFramework webFramework);

EXPORT void deleteWebFrameworkException(Exception exception);

EXPORT void deleteWebFrameworkJSONBuider(JSONBuilder builder);

EXPORT void deleteWebFrameworkJSONObject(JSONObject object);

EXPORT const char* getDataFromString(String string);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT WebFramework createWebFrameworkFromPath(const char* configPath, Exception* exception);

EXPORT WebFramework createWebFrameworkFromString(const char* serverConfiguration, const char* applicationDirectory, Exception* exception);

EXPORT WebFramework createWebFrameworkFromConfig(Config config, Exception* exception);

EXPORT Config createConfigFromPath(const char* configPath, Exception* exception);

EXPORT Config createConfigFromString(const char* serverConfiguration, const char* applicationDirectory, Exception* exception);

EXPORT JSONBuilder createJSONBuilder(JSONBuilder builder, Exception* exception);

EXPORT JSONObject createJSONObject(JSONObject object, Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT void startWebFrameworkServerCXX(WebFramework server, bool wait, void* onStartServer, Exception* exception);

EXPORT void startWebFrameworkServer(WebFramework server, bool wait, void (*onStartServer)(), Exception* exception);

EXPORT void stopWebFrameworkServer(WebFramework server, bool wait, Exception* exception);

EXPORT bool isServerRunning(WebFramework server, Exception* exception);

EXPORT const char* getWebFrameworkVersion();

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

EXPORT String getBasePath(Config config, Exception* exception);

EXPORT String build(JSONBuilder builder, Exception* exception);

EXPORT void appendObject(JSONBuilder builder, const char* key, JSONObject object, Exception* exception);

EXPORT void appendString(JSONBuilder builder, const char* key, const char* value, Exception* exception);

EXPORT void appendInteger(JSONBuilder builder, const char* key, int64_t value, Exception* exception);

EXPORT void appendUnsignedInteger(JSONBuilder builder, const char* key, uint64_t value, Exception* exception);

EXPORT void appendDouble(JSONBuilder builder, const char* key, double value, Exception* exception);

EXPORT void appendBoolean(JSONBuilder builder, const char* key, bool value, Exception* exception);

EXPORT void appendNull(JSONBuilder builder, const char* key, Exception* exception);

EXPORT void appendArray(JSONBuilder builder, const char* key, const JSONObject* objects, size_t size, Exception* exception);

EXPORT void setObject(JSONObject jsonObject, const char* key, JSONObject object, Exception* exception);

EXPORT void setString(JSONObject jsonObject, const char* key, const char* value, Exception* exception);

EXPORT void setInteger(JSONObject jsonObject, const char* key, int64_t value, Exception* exception);

EXPORT void setUnsignedInteger(JSONObject jsonObject, const char* key, uint64_t value, Exception* exception);

EXPORT void setDouble(JSONObject jsonObject, const char* key, double value, Exception* exception);

EXPORT void setBoolean(JSONObject jsonObject, const char* key, bool value, Exception* exception);

EXPORT void setNull(JSONObject jsonObject, const char* key, Exception* exception);

EXPORT void setArray(JSONObject jsonObject, const char* key, const JSONObject* objects, size_t size, Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT const char* getErrorMessage(Exception exception);
