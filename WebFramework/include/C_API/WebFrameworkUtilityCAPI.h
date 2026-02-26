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

EXPORT JsonObject createJsonObject(JsonObject object, Exception* exception);

EXPORT JsonBuilder createJsonBuilder(JsonBuilder builder, Exception* exception);

EXPORT JsonBuilder createJsonBuilderFromString(const char* jsonString, Exception* exception);

EXPORT JsonParser createJsonParser(JsonParser parser, Exception* exception);

EXPORT JsonParser createJsonParserFromString(const char* jsonString, Exception* exception);

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

EXPORT JsonObject getJsonObjectObject(JsonObject jsonObject, Exception* exception);

EXPORT const char* getJsonObjectString(JsonObject jsonObject, Exception* exception);

EXPORT int64_t getJsonObjectInteger(JsonObject jsonObject, Exception* exception);

EXPORT uint64_t getJsonObjectUnsignedInteger(JsonObject jsonObject, Exception* exception);

EXPORT double getJsonObjectDouble(JsonObject jsonObject, Exception* exception);

EXPORT bool getJsonObjectBoolean(JsonObject jsonObject, Exception* exception);

EXPORT void getJsonObjectNull(JsonObject jsonObject, Exception* exception);

EXPORT void getJsonObjectArray(JsonObject jsonObject, void(*addArrayValue)(JsonObject object, void* array), void* array, Exception* exception);

EXPORT bool tryGetJsonObjectObjectByKey(JsonObject jsonObject, const char* key, JsonObject* value, bool recursive, Exception* exception);

EXPORT bool tryGetJsonObjectStringByKey(JsonObject jsonObject, const char* key, String* value, bool recursive, Exception* exception);

EXPORT bool tryGetJsonObjectIntegerByKey(JsonObject jsonObject, const char* key, int64_t* value, bool recursive, Exception* exception);

EXPORT bool tryGetJsonObjectUnsignedIntegerByKey(JsonObject jsonObject, const char* key, uint64_t* value, bool recursive, Exception* exception);

EXPORT bool tryGetJsonObjectDoubleByKey(JsonObject jsonObject, const char* key, double* value, bool recursive, Exception* exception);

EXPORT bool tryGetJsonObjectBooleanByKey(JsonObject jsonObject, const char* key, bool* value, bool recursive, Exception* exception);

EXPORT bool tryGetJsonObjectNullByKey(JsonObject jsonObject, const char* key, bool recursive, Exception* exception);

EXPORT bool tryGetJsonObjectArrayByKey(JsonObject jsonObject, const char* key, void(*addArrayValue)(JsonObject object, void* array), void* array, bool recursive, Exception* exception);

EXPORT bool tryGetJsonObjectObject(JsonObject jsonObject, JsonObject* value, Exception* exception);

EXPORT bool tryGetJsonObjectString(JsonObject jsonObject, String* value, Exception* exception);

EXPORT bool tryGetJsonObjectInteger(JsonObject jsonObject, int64_t* value, Exception* exception);

EXPORT bool tryGetJsonObjectUnsignedInteger(JsonObject jsonObject, uint64_t* value, Exception* exception);

EXPORT bool tryGetJsonObjectDouble(JsonObject jsonObject, double* value, Exception* exception);

EXPORT bool tryGetJsonObjectBoolean(JsonObject jsonObject, bool* value, Exception* exception);

EXPORT bool tryGetJsonObjectNull(JsonObject jsonObject, Exception* exception);

EXPORT bool tryGetJsonObjectArray(JsonObject jsonObject, void(*addArrayValue)(JsonObject object, void* array), void* array, Exception* exception);

EXPORT void setJsonObjectObject(JsonObject jsonObject, JsonObject value, Exception* exception);

EXPORT void setJsonObjectString(JsonObject jsonObject, const char* value, Exception* exception);

EXPORT void setJsonObjectInteger(JsonObject jsonObject, int64_t value, Exception* exception);

EXPORT void setJsonObjectUnsignedInteger(JsonObject jsonObject, uint64_t value, Exception* exception);

EXPORT void setJsonObjectDouble(JsonObject jsonObject, double value, Exception* exception);

EXPORT void setJsonObjectBoolean(JsonObject jsonObject, bool value, Exception* exception);

EXPORT void setJsonObjectNull(JsonObject jsonObject, Exception* exception);

EXPORT void setJsonObjectArray(JsonObject jsonObject, JsonObject* objects, size_t size, Exception* exception);

EXPORT String jsonObjectToString(JsonObject jsonObject, Exception* exception);

EXPORT String buildJsonBuilder(JsonBuilder builder, Exception* exception);

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

EXPORT void appendJsonBuilderObject(JsonBuilder builder, const char* key, JsonObject object, Exception* exception);

EXPORT void appendJsonBuilderString(JsonBuilder builder, const char* key, const char* value, Exception* exception);

EXPORT void appendJsonBuilderInteger(JsonBuilder builder, const char* key, int64_t value, Exception* exception);

EXPORT void appendJsonBuilderUnsignedInteger(JsonBuilder builder, const char* key, uint64_t value, Exception* exception);

EXPORT void appendJsonBuilderDouble(JsonBuilder builder, const char* key, double value, Exception* exception);

EXPORT void appendJsonBuilderBoolean(JsonBuilder builder, const char* key, bool value, Exception* exception);

EXPORT void appendJsonBuilderNull(JsonBuilder builder, const char* key, Exception* exception);

EXPORT void appendJsonBuilderArray(JsonBuilder builder, const char* key, JsonObject* objects, size_t size, Exception* exception);

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

EXPORT JsonObject getJsonParserObject(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT const char* getJsonParserString(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT int64_t getJsonParserInteger(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT uint64_t getJsonParserUnsignedInteger(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT double getJsonParserDouble(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool getJsonParserBoolean(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT void getJsonParserNull(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT void getJsonParserArray(JsonParser parser, const char* key, void(*addArrayValue)(JsonObject object, void* array), void* array, bool recursive, Exception* exception);

EXPORT bool tryGetJsonParserObject(JsonParser parser, const char* key, JsonObject* value, bool recursive, Exception* exception);

EXPORT bool tryGetJsonParserString(JsonParser parser, const char* key, String* value, bool recursive, Exception* exception);

EXPORT bool tryGetJsonParserInteger(JsonParser parser, const char* key, int64_t* value, bool recursive, Exception* exception);

EXPORT bool tryGetJsonParserUnsignedInteger(JsonParser parser, const char* key, uint64_t* value, bool recursive, Exception* exception);

EXPORT bool tryGetJsonParserDouble(JsonParser parser, const char* key, double* value, bool recursive, Exception* exception);

EXPORT bool tryGetJsonParserBoolean(JsonParser parser, const char* key, bool* value, bool recursive, Exception* exception);

EXPORT bool tryGetJsonParserNull(JsonParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool tryGetJsonParserArray(JsonParser parser, const char* key, void(*addArrayValue)(JsonObject object, void* array), void* array, bool recursive, Exception* exception);

EXPORT const char* getJsonParserRawData(JsonParser parser, Exception* exception);

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

EXPORT void deleteWebFrameworkJsonObject(JsonObject object);

EXPORT void deleteWebFrameworkJsonBuilder(JsonBuilder builder);

EXPORT void deleteWebFrameworkJsonParser(JsonParser parser);

EXPORT void deleteWebFrameworkSQLValue(SqlValueObject values);
