#pragma once

#include "WebFrameworkCAPI.h"

typedef void* JSONBuilder;
typedef void* JSONParser; 
typedef void* JSONObject;
typedef void* ExecutorSettings;
typedef void* DatabaseObject;
typedef void* TableObject;
typedef void* SQLResultObject;
typedef void* SQLValueObject;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT JSONObject createJSONObject(JSONObject object, Exception* exception);

EXPORT JSONBuilder createJSONBuilder(JSONBuilder builder, Exception* exception);

EXPORT JSONBuilder createJSONBuilderFromString(const char* jsonString, Exception* exception);

EXPORT JSONParser createJSONParser(JSONParser parser, Exception* exception);

EXPORT JSONParser createJSONParserFromString(const char* jsonString, Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT String buildJSONBuilder(JSONBuilder builder, Exception* exception);

EXPORT void standardJsonBuilder(JSONBuilder builder, Exception* exception);

EXPORT void minimizeJsonBuilder(JSONBuilder builder, Exception* exception);

EXPORT void setJSONObjectObject(JSONObject jsonObject, const char* key, JSONObject object, Exception* exception);

EXPORT void setJSONObjectString(JSONObject jsonObject, const char* key, const char* value, Exception* exception);

EXPORT void setJSONObjectInteger(JSONObject jsonObject, const char* key, int64_t value, Exception* exception);

EXPORT void setJSONObjectUnsignedInteger(JSONObject jsonObject, const char* key, uint64_t value, Exception* exception);

EXPORT void setJSONObjectDouble(JSONObject jsonObject, const char* key, double value, Exception* exception);

EXPORT void setJSONObjectBoolean(JSONObject jsonObject, const char* key, bool value, Exception* exception);

EXPORT void setJSONObjectNull(JSONObject jsonObject, const char* key, Exception* exception);

EXPORT void setJSONObjectArray(JSONObject jsonObject, const char* key, JSONObject* objects, size_t size, Exception* exception);

EXPORT bool containsJsonBuilderObject(JSONBuilder builder, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonBuilderString(JSONBuilder builder, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonBuilderInteger(JSONBuilder builder, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonBuilderUnsignedInteger(JSONBuilder builder, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonBuilderDouble(JSONBuilder builder, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonBuilderBoolean(JSONBuilder builder, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonBuilderNull(JSONBuilder builder, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonBuilderArray(JSONBuilder builder, const char* key, bool recursive, Exception* exception);

EXPORT void appendJSONBuilderObject(JSONBuilder builder, const char* key, JSONObject object, Exception* exception);

EXPORT void appendJSONBuilderString(JSONBuilder builder, const char* key, const char* value, Exception* exception);

EXPORT void appendJSONBuilderInteger(JSONBuilder builder, const char* key, int64_t value, Exception* exception);

EXPORT void appendJSONBuilderUnsignedInteger(JSONBuilder builder, const char* key, uint64_t value, Exception* exception);

EXPORT void appendJSONBuilderDouble(JSONBuilder builder, const char* key, double value, Exception* exception);

EXPORT void appendJSONBuilderBoolean(JSONBuilder builder, const char* key, bool value, Exception* exception);

EXPORT void appendJSONBuilderNull(JSONBuilder builder, const char* key, Exception* exception);

EXPORT void appendJSONBuilderArray(JSONBuilder builder, const char* key, JSONObject* objects, size_t size, Exception* exception);

EXPORT bool containsJsonParserObject(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonParserString(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonParserInteger(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonParserUnsignedInteger(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonParserDouble(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonParserBoolean(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonParserNull(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool containsJsonParserArray(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT void overrideJsonParserObject(JSONParser parser, const char* key, JSONObject value, bool recursive, Exception* exception);

EXPORT void overrideJsonParserString(JSONParser parser, const char* key, const char* value, bool recursive, Exception* exception);

EXPORT void overrideJsonParserInteger(JSONParser parser, const char* key, int64_t value, bool recursive, Exception* exception);

EXPORT void overrideJsonParserUnsignedInteger(JSONParser parser, const char* key, uint64_t value, bool recursive, Exception* exception);

EXPORT void overrideJsonParserDouble(JSONParser parser, const char* key, double value, bool recursive, Exception* exception);

EXPORT void overrideJsonParserBoolean(JSONParser parser, const char* key, bool value, bool recursive, Exception* exception);

EXPORT void overrideJsonParserNull(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT void overrideJsonParserArray(JSONParser parser, const char* key, JSONObject* value, size_t size, bool recursive, Exception* exception);

EXPORT JSONObject getJSONParserObject(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT const char* getJSONParserString(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT int64_t getJSONParserInteger(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT uint64_t getJSONParserUnsignedInteger(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT double getJSONParserDouble(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool getJSONParserBoolean(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT void getJSONParserNull(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT void getJSONParserArray(JSONParser parser, const char* key, void(*addArrayValue)(JSONObject object, void* array), void* array, bool recursive, Exception* exception);

EXPORT bool tryGetJSONParserObject(JSONParser parser, const char* key, JSONObject* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONParserString(JSONParser parser, const char* key, String* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONParserInteger(JSONParser parser, const char* key, int64_t* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONParserUnsignedInteger(JSONParser parser, const char* key, uint64_t* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONParserDouble(JSONParser parser, const char* key, double* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONParserBoolean(JSONParser parser, const char* key, bool* value, bool recursive, Exception* exception);

EXPORT bool tryGetJSONParserNull(JSONParser parser, const char* key, bool recursive, Exception* exception);

EXPORT bool tryGetJSONParserArray(JSONParser parser, const char* key, void(*addArrayValue)(JSONObject object, void* array), void* array, bool recursive, Exception* exception);

EXPORT const char* getJSONParserRawData(JSONParser parser, Exception* exception);

EXPORT TableObject getOrCreateTable(DatabaseObject database, const char* tableName, const char* createTableQuery, Exception* exception);

EXPORT TableObject getTable(DatabaseObject database, const char* tableName, Exception* exception);

EXPORT bool containsTable(DatabaseObject database, const char* tableName, TableObject* table, Exception* exception);

EXPORT const char* getDatabaseName(DatabaseObject database, Exception* exception);

EXPORT const char* getDatabaseFileName(DatabaseObject database, Exception* exception);

EXPORT SQLResultObject executeQuery(TableObject table, const char* query, SQLValueObject* values, size_t size, Exception* exception);

EXPORT void deleteSQLResult(TableObject table, SQLResultObject result, Exception* exception);

EXPORT const char* getTableName(TableObject table, Exception* exception);

EXPORT SQLValueObject createSQLValue(Exception* exception);

EXPORT void setSQLValueInt(SQLValueObject sqlValue, int64_t value, Exception* exception);

EXPORT void setSQLValueDouble(SQLValueObject sqlValue, double value, Exception* exception);

EXPORT void setSQLValueString(SQLValueObject sqlValue, const char* value, Exception* exception);

EXPORT void setSQLValueBool(SQLValueObject sqlValue, bool value, Exception* exception);

EXPORT void setSQLValueNull(SQLValueObject sqlValue, Exception* exception);

EXPORT void setSQLValueBlob(SQLValueObject sqlValue, const uint8_t* value, size_t size, Exception* exception);

EXPORT int64_t getSQLValueInt(SQLValueObject sqlValue, Exception* exception);

EXPORT double getSQLValueDouble(SQLValueObject sqlValue, Exception* exception);

EXPORT const char* getSQLValueString(SQLValueObject sqlValue, Exception* exception);

EXPORT bool getSQLValueBool(SQLValueObject sqlValue, Exception* exception);

EXPORT const uint8_t* getSQLValueBlob(SQLValueObject sqlValue, size_t* size, Exception* exception);

EXPORT int getSQLValueType(SQLValueObject sqlValue, Exception* exception);

EXPORT size_t getSQLResultSize(SQLResultObject result, Exception* exception);

EXPORT void iterateSQLResult(SQLResultObject result, void(*initBuffer)(size_t size, void* buffer), void(*callback)(const char** columnNames, const void** columnValues, size_t size, size_t index, void* buffer), void* buffer, Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT String generateWebFrameworkUUID(Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT void deleteWebFrameworkJSONObject(JSONObject object);

EXPORT void deleteWebFrameworkJSONBuilder(JSONBuilder builder);

EXPORT void deleteWebFrameworkJSONParser(JSONParser parser);

EXPORT void deleteWebFrameworkSQLValue(SQLValueObject values);
