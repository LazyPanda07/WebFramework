#pragma once

#include "JSONObject.h"

WebFrameworkException createJSONParser(JSONParser* parser);

WebFrameworkException copyJSONParser(JSONParser* parser, const JSONParser* other);

WebFrameworkException createJSONParserFromString(const char* jsonString, JSONParser* parser);

WebFrameworkException getJSONParserObject(JSONParser implementation, const char* key, bool recursive, JSONObject* result);

WebFrameworkException getJSONParserString(JSONParser implementation, const char* key, bool recursive, const char** result);

WebFrameworkException getJSONParserInteger(JSONParser implementation, const char* key, bool recursive, int64_t* result);

WebFrameworkException getJSONParserUnsignedInteger(JSONParser implementation, const char* key, bool recursive, uint64_t* result);

WebFrameworkException getJSONParserDouble(JSONParser implementation, const char* key, bool recursive, double* result);

WebFrameworkException getJSONParserBoolean(JSONParser implementation, const char* key, bool recursive, bool* result);

WebFrameworkException getJSONParserNull(JSONParser implementation, const char* key, bool recursive, bool* result);

WebFrameworkException getJSONParserArray(JSONParser implementation, const char* key, JSONArray* array, bool recursive);

bool tryGetJSONParserObject(JSONParser implementation, const char* key, JSONObject* value, bool recursive, WebFrameworkException* exception);

bool tryGetJSONParserString(JSONParser implementation, const char* key, const char** value, bool recursive, WebFrameworkException* exception);

bool tryGetJSONParserInteger(JSONParser implementation, const char* key, int64_t* value, bool recursive, WebFrameworkException* exception);

bool tryGetJSONParserUnsignedInteger(JSONParser implementation, const char* key, uint64_t* value, bool recursive, WebFrameworkException* exception);

bool tryGetJSONParserDouble(JSONParser implementation, const char* key, double* value, bool recursive, WebFrameworkException* exception);

bool tryGetJSONParserBoolean(JSONParser implementation, const char* key, bool* value, bool recursive, WebFrameworkException* exception);

bool tryGetJSONParserNull(JSONParser implementation, const char* key, bool recursive, WebFrameworkException* exception);

bool tryGetJSONParserArray(JSONParser implementation, const char* key, JSONArray* array, bool recursive, WebFrameworkException* exception);
