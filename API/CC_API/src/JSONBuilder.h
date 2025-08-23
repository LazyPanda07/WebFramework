#pragma once

#include "JSONObject.h"

WebFrameworkException createJSONBuilder(JSONBuilder* builder);

WebFrameworkException copyJSONBuilder(JSONBuilder* builder, JSONBuilder other);

WebFrameworkException buildJSONBuilder(JSONBuilder implementation, WebFrameworkString* result);

WebFrameworkException appendJSONBuilderObject(JSONBuilder implementation, const char* key, JSONObject* object);

WebFrameworkException appendJSONBuilderString(JSONBuilder implementation, const char* key, const char* value);

WebFrameworkException appendJSONBuilderInteger(JSONBuilder implementation, const char* key, int64_t value);

WebFrameworkException appendJSONBuilderUnsignedInteger(JSONBuilder implementation, const char* key, uint64_t value);

WebFrameworkException appendJSONBuilderDouble(JSONBuilder implementation, const char* key, double value);

WebFrameworkException appendJSONBuilderBoolean(JSONBuilder implementation, const char* key, bool value);

WebFrameworkException appendJSONBuilderNull(JSONBuilder implementation, const char* key);

WebFrameworkException appendJSONBuilderArray(JSONBuilder implementation, const char* key, const JSONArray* array);
