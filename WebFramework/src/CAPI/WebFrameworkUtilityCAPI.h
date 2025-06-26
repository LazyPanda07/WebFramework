#pragma once

#include "WebFrameworkCAPI.h"

typedef void* JSONObject;
typedef void* JSONBuilder;
typedef void* JSONParser;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT JSONObject createJSONObject(JSONObject object, Exception* exception);

EXPORT JSONBuilder createJSONBuilder(JSONBuilder builder, Exception* exception);

EXPORT JSONParser createJSONParser(JSONParser parser, Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT String build(JSONBuilder builder, Exception* exception);

EXPORT void setObject(JSONObject jsonObject, const char* key, JSONObject object, Exception* exception);

EXPORT void setString(JSONObject jsonObject, const char* key, const char* value, Exception* exception);

EXPORT void setInteger(JSONObject jsonObject, const char* key, int64_t value, Exception* exception);

EXPORT void setUnsignedInteger(JSONObject jsonObject, const char* key, uint64_t value, Exception* exception);

EXPORT void setDouble(JSONObject jsonObject, const char* key, double value, Exception* exception);

EXPORT void setBoolean(JSONObject jsonObject, const char* key, bool value, Exception* exception);

EXPORT void setNull(JSONObject jsonObject, const char* key, Exception* exception);

EXPORT void setArray(JSONObject jsonObject, const char* key, const JSONObject* objects, size_t size, Exception* exception);

EXPORT void appendObject(JSONBuilder builder, const char* key, JSONObject object, Exception* exception);

EXPORT void appendString(JSONBuilder builder, const char* key, const char* value, Exception* exception);

EXPORT void appendInteger(JSONBuilder builder, const char* key, int64_t value, Exception* exception);

EXPORT void appendUnsignedInteger(JSONBuilder builder, const char* key, uint64_t value, Exception* exception);

EXPORT void appendDouble(JSONBuilder builder, const char* key, double value, Exception* exception);

EXPORT void appendBoolean(JSONBuilder builder, const char* key, bool value, Exception* exception);

EXPORT void appendNull(JSONBuilder builder, const char* key, Exception* exception);

EXPORT void appendArray(JSONBuilder builder, const char* key, const JSONObject* objects, size_t size, Exception* exception);

EXPORT JSONObject getObject(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT const char* getString(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT int64_t getInteger(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT uint64_t getUnsignedInteger(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT double getDouble(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool getBoolean(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT void getNull(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT void getArray(JSONParser parser, const char* key, void(*addArrayValue)(JSONObject object, void* array), void* array, bool recursive, Exception* exception);

EXPORT bool tryGetObject(JSONParser parser, const char* key, JSONObject* value, bool recursive, Exception* exception);

EXPORT bool tryGetString(JSONParser parser, const char* key, String* value, bool recursive, Exception* exception);

EXPORT bool tryGetInteger(JSONParser parser, const char* key, int64_t* value, bool recursive, Exception* exception);

EXPORT bool tryGetUnsignedInteger(JSONParser parser, const char* key, uint64_t* value, bool recursive, Exception* exception);

EXPORT bool tryGetDouble(JSONParser parser, const char* key, double* value, bool recursive, Exception* exception);

EXPORT bool tryGetBoolean(JSONParser parser, const char* key, bool* value, bool recursive, Exception* exception);

EXPORT bool tryGetNull(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool tryGetArray(JSONParser parser, const char* key, void(*addArrayValue)(JSONObject object, void* array), void* array, bool recursive, Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT void deleteWebFrameworkJSONObject(JSONObject object);

EXPORT void deleteWebFrameworkJSONBuider(JSONBuilder builder);

EXPORT void deleteWebFrameworkJSONParser(JSONParser parser);
