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

EXPORT JSONParser createJSONParser(JSONParser parser, Exception* exception);

EXPORT JSONParser createJSONParserFromString(const char* jsonString, Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT String build(JSONBuilder builder, Exception* exception);

EXPORT void setJSONObjectObject(JSONObject jsonObject, const char* key, JSONObject object, Exception* exception);

EXPORT void setJSONObjectString(JSONObject jsonObject, const char* key, const char* value, Exception* exception);

EXPORT void setJSONObjectInteger(JSONObject jsonObject, const char* key, int64_t value, Exception* exception);

EXPORT void setJSONObjectUnsignedInteger(JSONObject jsonObject, const char* key, uint64_t value, Exception* exception);

EXPORT void setJSONObjectDouble(JSONObject jsonObject, const char* key, double value, Exception* exception);

EXPORT void setJSONObjectBoolean(JSONObject jsonObject, const char* key, bool value, Exception* exception);

EXPORT void setJSONObjectNull(JSONObject jsonObject, const char* key, Exception* exception);

EXPORT void setJSONObjectArray(JSONObject jsonObject, const char* key, const JSONObject* objects, size_t size, Exception* exception);

EXPORT void appendJSONBuilderObject(JSONBuilder builder, const char* key, JSONObject object, Exception* exception);

EXPORT void appendJSONBuilderString(JSONBuilder builder, const char* key, const char* value, Exception* exception);

EXPORT void appendJSONBuilderInteger(JSONBuilder builder, const char* key, int64_t value, Exception* exception);

EXPORT void appendJSONBuilderUnsignedInteger(JSONBuilder builder, const char* key, uint64_t value, Exception* exception);

EXPORT void appendJSONBuilderDouble(JSONBuilder builder, const char* key, double value, Exception* exception);

EXPORT void appendJSONBuilderBoolean(JSONBuilder builder, const char* key, bool value, Exception* exception);

EXPORT void appendJSONBuilderNull(JSONBuilder builder, const char* key, Exception* exception);

EXPORT void appendJSONBuilderArray(JSONBuilder builder, const char* key, const JSONObject* objects, size_t size, Exception* exception);

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

EXPORT TableObject getOrCreateTable(DatabaseObject database, const char* tableName, const char* createTableQuery, Exception* exception);

EXPORT TableObject getTable(DatabaseObject database, const char* tableName, Exception* exception);

EXPORT bool containsTable(DatabaseObject database, const char* tableName, TableObject* table, Exception* exception);

EXPORT const char* getDatabaseName(DatabaseObject database, Exception* exception);

EXPORT const char* getDatabaseFileName(DatabaseObject database, Exception* exception);

EXPORT SQLResultObject executeQuery(TableObject table, const char* query, SQLValueObject* values, size_t size, Exception* exception);

EXPORT void deleteSQLResult(TableObject table, SQLResultObject result, Exception* exception);

EXPORT const char* getTableName(TableObject table, Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT String generateWebFrameworkUUID(Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT void deleteWebFrameworkJSONObject(JSONObject object);

EXPORT void deleteWebFrameworkJSONBuider(JSONBuilder builder);

EXPORT void deleteWebFrameworkJSONParser(JSONParser parser);
