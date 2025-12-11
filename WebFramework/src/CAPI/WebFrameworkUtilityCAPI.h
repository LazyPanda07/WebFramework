#pragma once

#include "WebFrameworkCAPI.h"

typedef void* JsonBuilder;
typedef void* JsonParser; 
typedef void* JsonObject;
typedef void* ExecutorSettings;
typedef void* DatabaseObject;
typedef void* TableObject;
typedef void* SqlResultObject;
typedef void* SqlValueObject;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT JsonObject createJSONObject(JsonObject object, Exception* exception);

EXPORT JsonBuilder createJSONBuilder(JsonBuilder builder, Exception* exception);

EXPORT JsonBuilder createJSONBuilderFromString(const char* jsonString, Exception* exception);

EXPORT JsonParser createJSONParser(JsonParser parser, Exception* exception);

EXPORT JsonParser createJSONParserFromString(const char* jsonString, Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT JsonObject accessIndexOperatorJsonObject(JsonObject jsonObject, size_t index, Exception* exception);

EXPORT JsonObject accessKeyOperatorJsonObject(JsonObject jsonObject, const char* key, Exception* exception);

EXPORT JsonObject emplaceBackObject(JsonObject jsonObject, JsonObject value, Exception* exception);

EXPORT JsonObject emplaceBackString(JsonObject jsonObject, const char* value, Exception* exception);

EXPORT JsonObject emplaceBackInteger(JsonObject jsonObject, int64_t value, Exception* exception);

EXPORT JsonObject emplaceBackUnsignedInteger(JsonObject jsonObject, uint64_t value, Exception* exception);

EXPORT JsonObject emplaceBackDouble(JsonObject jsonObject, double value, Exception* exception);

EXPORT JsonObject emplaceBackBoolean(JsonObject jsonObject, bool value, Exception* exception);

EXPORT JsonObject emplaceBackNull(JsonObject jsonObject, Exception* exception);

EXPORT JsonObject emplaceBackArray(JsonObject jsonObject, JsonObject* objects, size_t size, Exception* exception);

EXPORT bool isObject(JsonObject jsonObject, Exception* exception);

EXPORT bool isString(JsonObject jsonObject, Exception* exception);

EXPORT bool isInteger(JsonObject jsonObject, Exception* exception);

EXPORT bool isUnsignedInteger(JsonObject jsonObject, Exception* exception);

EXPORT bool isDouble(JsonObject jsonObject, Exception* exception);

EXPORT bool isBoolean(JsonObject jsonObject, Exception* exception);

EXPORT bool isNull(JsonObject jsonObject, Exception* exception);

EXPORT bool isArray(JsonObject jsonObject, Exception* exception);

EXPORT bool containsJsonObjectObject(JsonObject jsonObject, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonObjectString(JsonObject jsonObject, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonObjectInteger(JsonObject jsonObject, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonObjectUnsignedInteger(JsonObject jsonObject, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonObjectDouble(JsonObject jsonObject, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonObjectBoolean(JsonObject jsonObject, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonObjectNull(JsonObject jsonObject, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonObjectArray(JsonObject jsonObject, const char* key, bool recursive, Exception* exception);

EXPORT size_t sizeJsonObject(JsonObject jsonObject, Exception* exception);

EXPORT JsonObject getJSONObjectObject(JsonObject jsonObject, Exception* exception);

EXPORT const char* getJSONObjectString(JsonObject jsonObject, Exception* exception);

EXPORT int64_t getJSONObjectInteger(JsonObject jsonObject, Exception* exception);

EXPORT uint64_t getJSONObjectUnsignedInteger(JsonObject jsonObject, Exception* exception);

EXPORT double getJSONObjectDouble(JsonObject jsonObject, Exception* exception);

EXPORT bool getJSONObjectBoolean(JsonObject jsonObject, Exception* exception);

EXPORT void getJSONObjectNull(JsonObject jsonObject, Exception* exception);

EXPORT void getJSONObjectArray(JsonObject jsonObject, void(*addArrayValue)(JsonObject object, void* array), void* array, Exception* exception);

EXPORT bool tryGetJSONObjectObjectByKey(JsonObject jsonObject, const char* key, JsonObject* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONObjectStringByKey(JsonObject jsonObject, const char* key, String* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONObjectIntegerByKey(JsonObject jsonObject, const char* key, int64_t* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONObjectUnsignedIntegerByKey(JsonObject jsonObject, const char* key, uint64_t* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONObjectDoubleByKey(JsonObject jsonObject, const char* key, double* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONObjectBooleanByKey(JsonObject jsonObject, const char* key, bool* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONObjectNullByKey(JsonObject jsonObject, const char* key, bool recursive, Exception* exception);

EXPORT bool tryGetJSONObjectArrayByKey(JsonObject jsonObject, const char* key, void(*addArrayValue)(JsonObject object, void* array), void* array, bool recursive, Exception* exception);

EXPORT bool tryGetJSONObjectObject(JsonObject jsonObject, JsonObject* value, Exception* exception);

EXPORT bool tryGetJSONObjectString(JsonObject jsonObject, String* value, Exception* exception);

EXPORT bool tryGetJSONObjectInteger(JsonObject jsonObject, int64_t* value, Exception* exception);

EXPORT bool tryGetJSONObjectUnsignedInteger(JsonObject jsonObject, uint64_t* value, Exception* exception);

EXPORT bool tryGetJSONObjectDouble(JsonObject jsonObject, double* value, Exception* exception);

EXPORT bool tryGetJSONObjectBoolean(JsonObject jsonObject, bool* value, Exception* exception);

EXPORT bool tryGetJSONObjectNull(JsonObject jsonObject, Exception* exception);

EXPORT bool tryGetJSONObjectArray(JsonObject jsonObject, void(*addArrayValue)(JsonObject object, void* array), void* array, Exception* exception);

EXPORT void setJSONObjectObject(JsonObject jsonObject, JsonObject value, Exception* exception);

EXPORT void setJSONObjectString(JsonObject jsonObject, const char* value, Exception* exception);

EXPORT void setJSONObjectInteger(JsonObject jsonObject, int64_t value, Exception* exception);

EXPORT void setJSONObjectUnsignedInteger(JsonObject jsonObject, uint64_t value, Exception* exception);

EXPORT void setJSONObjectDouble(JsonObject jsonObject, double value, Exception* exception);

EXPORT void setJSONObjectBoolean(JsonObject jsonObject, bool value, Exception* exception);

EXPORT void setJSONObjectNull(JsonObject jsonObject, Exception* exception);

EXPORT void setJSONObjectArray(JsonObject jsonObject, JsonObject* objects, size_t size, Exception* exception);

EXPORT String jsonObjectToString(JsonObject jsonObject, Exception* exception);

EXPORT String buildJSONBuilder(JsonBuilder builder, Exception* exception);

EXPORT void standardJsonBuilder(JsonBuilder builder, Exception* exception);

EXPORT void minimizeJsonBuilder(JsonBuilder builder, Exception* exception);

EXPORT bool containsJsonBuilderObject(JsonBuilder builder, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonBuilderString(JsonBuilder builder, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonBuilderInteger(JsonBuilder builder, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonBuilderUnsignedInteger(JsonBuilder builder, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonBuilderDouble(JsonBuilder builder, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonBuilderBoolean(JsonBuilder builder, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonBuilderNull(JsonBuilder builder, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonBuilderArray(JsonBuilder builder, const char* key, bool recursive, Exception* exception);

EXPORT void appendJSONBuilderObject(JsonBuilder builder, const char* key, JsonObject object, Exception* exception);

EXPORT void appendJSONBuilderString(JsonBuilder builder, const char* key, const char* value, Exception* exception);

EXPORT void appendJSONBuilderInteger(JsonBuilder builder, const char* key, int64_t value, Exception* exception);

EXPORT void appendJSONBuilderUnsignedInteger(JsonBuilder builder, const char* key, uint64_t value, Exception* exception);

EXPORT void appendJSONBuilderDouble(JsonBuilder builder, const char* key, double value, Exception* exception);

EXPORT void appendJSONBuilderBoolean(JsonBuilder builder, const char* key, bool value, Exception* exception);

EXPORT void appendJSONBuilderNull(JsonBuilder builder, const char* key, Exception* exception);

EXPORT void appendJSONBuilderArray(JsonBuilder builder, const char* key, JsonObject* objects, size_t size, Exception* exception);

EXPORT JsonObject accessKeyOperatorJsonBuilder(JsonBuilder builder, const char* key, Exception* exception);

EXPORT bool containsJsonParserObject(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonParserString(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonParserInteger(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonParserUnsignedInteger(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonParserDouble(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonParserBoolean(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonParserNull(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonParserArray(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT void overrideJsonParserObject(JsonParser parser, const char* key, JsonObject value, bool recursive, Exception* exception);

EXPORT void overrideJsonParserString(JsonParser parser, const char* key, const char* value, bool recursive, Exception* exception);

EXPORT void overrideJsonParserInteger(JsonParser parser, const char* key, int64_t value, bool recursive, Exception* exception);

EXPORT void overrideJsonParserUnsignedInteger(JsonParser parser, const char* key, uint64_t value, bool recursive, Exception* exception);

EXPORT void overrideJsonParserDouble(JsonParser parser, const char* key, double value, bool recursive, Exception* exception);

EXPORT void overrideJsonParserBoolean(JsonParser parser, const char* key, bool value, bool recursive, Exception* exception);

EXPORT void overrideJsonParserNull(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT void overrideJsonParserArray(JsonParser parser, const char* key, JsonObject* value, size_t size, bool recursive, Exception* exception);

EXPORT JsonObject getJSONParserObject(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT const char* getJSONParserString(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT int64_t getJSONParserInteger(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT uint64_t getJSONParserUnsignedInteger(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT double getJSONParserDouble(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool getJSONParserBoolean(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT void getJSONParserNull(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT void getJSONParserArray(JsonParser parser, const char* key, void(*addArrayValue)(JsonObject object, void* array), void* array, bool recursive, Exception* exception);

EXPORT bool tryGetJSONParserObject(JsonParser parser, const char* key, JsonObject* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONParserString(JsonParser parser, const char* key, String* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONParserInteger(JsonParser parser, const char* key, int64_t* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONParserUnsignedInteger(JsonParser parser, const char* key, uint64_t* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONParserDouble(JsonParser parser, const char* key, double* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONParserBoolean(JsonParser parser, const char* key, bool* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONParserNull(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool tryGetJSONParserArray(JsonParser parser, const char* key, void(*addArrayValue)(JsonObject object, void* array), void* array, bool recursive, Exception* exception);

EXPORT const char* getJSONParserRawData(JsonParser parser, Exception* exception);

EXPORT TableObject getOrCreateTable(DatabaseObject database, const char* tableName, const char* createTableQuery, Exception* exception);

EXPORT TableObject getTable(DatabaseObject database, const char* tableName, Exception* exception);

EXPORT bool containsTable(DatabaseObject database, const char* tableName, TableObject* table, Exception* exception);

EXPORT const char* getDatabaseName(DatabaseObject database, Exception* exception);

EXPORT const char* getDatabaseFileName(DatabaseObject database, Exception* exception);

EXPORT SqlResultObject executeQuery(TableObject table, const char* query, SqlValueObject* values, size_t size, Exception* exception);

EXPORT void deleteSQLResult(TableObject table, SqlResultObject result, Exception* exception);

EXPORT const char* getTableName(TableObject table, Exception* exception);

EXPORT SqlValueObject createSQLValue(Exception* exception);

EXPORT void setSQLValueInt(SqlValueObject sqlValue, int64_t value, Exception* exception);

EXPORT void setSQLValueDouble(SqlValueObject sqlValue, double value, Exception* exception);

EXPORT void setSQLValueString(SqlValueObject sqlValue, const char* value, Exception* exception);

EXPORT void setSQLValueBool(SqlValueObject sqlValue, bool value, Exception* exception);

EXPORT void setSQLValueNull(SqlValueObject sqlValue, Exception* exception);

EXPORT void setSQLValueBlob(SqlValueObject sqlValue, const uint8_t* value, size_t size, Exception* exception);

EXPORT int64_t getSQLValueInt(SqlValueObject sqlValue, Exception* exception);

EXPORT double getSQLValueDouble(SqlValueObject sqlValue, Exception* exception);

EXPORT const char* getSQLValueString(SqlValueObject sqlValue, Exception* exception);

EXPORT bool getSQLValueBool(SqlValueObject sqlValue, Exception* exception);

EXPORT const uint8_t* getSQLValueBlob(SqlValueObject sqlValue, size_t* size, Exception* exception);

EXPORT int getSQLValueType(SqlValueObject sqlValue, Exception* exception);

EXPORT size_t getSQLResultSize(SqlResultObject result, Exception* exception);

EXPORT void iterateSQLResult(SqlResultObject result, void(*initBuffer)(size_t size, void* buffer), void(*callback)(const char** columnNames, const void** columnValues, size_t size, size_t index, void* buffer), void* buffer, Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT String generateWebFrameworkUUID(Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT void deleteWebFrameworkJSONObject(JsonObject object);

EXPORT void deleteWebFrameworkJSONBuilder(JsonBuilder builder);

EXPORT void deleteWebFrameworkJSONParser(JsonParser parser);

EXPORT void deleteWebFrameworkSQLValue(SqlValueObject values);
