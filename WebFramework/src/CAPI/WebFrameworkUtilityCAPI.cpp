#include "WebFrameworkUtilityCAPI.h"

#include <Log.h>
#include <JSONBuilder.h>
#include <JSONParser.h>
#include <UUID.h>

#include "Utility/JSONSettingsParser.h"
#include "DatabaseInterfaces/IDatabase.h"
#include "Databases/SQLValueImplementation.h"

#define LOG_EXCEPTION() if (Log::isValid()) { Log::error("Exception: {} in {} function", "C_API", e.what(), __func__); }
#define CREATE_EXCEPTION() *exception = new std::runtime_error(e.what())
#define LOG_AND_CREATE_EXCEPTION() LOG_EXCEPTION(); CREATE_EXCEPTION()
#define UNEXPECTED_EXCEPTION() if (Log::isValid()) { Log::error("Somethind went wrong", "C_API"); } *exception = new std::runtime_error(std::format("Something went wrong in file: {} on line: {}", __FILE__, __LINE__));

JSONObject createJSONObject(JSONObject object, Exception* exception)
{
	try
	{
		return object ?
			new json::utility::jsonObject(*static_cast<json::utility::jsonObject*>(object)) :
			new json::utility::jsonObject();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

JSONBuilder createJSONBuilder(JSONBuilder builder, Exception* exception)
{
	try
	{
		return builder ?
			new json::JSONBuilder(*static_cast<json::JSONBuilder*>(builder)) :
			new json::JSONBuilder(CP_UTF8);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

JSONParser createJSONParser(JSONParser parser, Exception* exception)
{
	try
	{
		return parser ?
			new json::JSONParser(*static_cast<json::JSONParser*>(parser)) :
			new json::JSONParser();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

JSONParser createJSONParserFromString(const char* jsonString, Exception* exception)
{
	try
	{
		return new json::JSONParser(jsonString);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setJSONObjectObject(JSONObject jsonObject, const char* key, JSONObject object, Exception* exception)
{
	try
	{
		static_cast<json::utility::jsonObject*>(jsonObject)->setObject(key, *static_cast<json::utility::jsonObject*>(object));
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setJSONObjectString(JSONObject jsonObject, const char* key, const char* value, Exception* exception)
{
	try
	{
		static_cast<json::utility::jsonObject*>(jsonObject)->setString(key, value);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setJSONObjectInteger(JSONObject jsonObject, const char* key, int64_t value, Exception* exception)
{
	try
	{
		static_cast<json::utility::jsonObject*>(jsonObject)->setInt(key, value);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setJSONObjectUnsignedInteger(JSONObject jsonObject, const char* key, uint64_t value, Exception* exception)
{
	try
	{
		static_cast<json::utility::jsonObject*>(jsonObject)->setUnsignedInt(key, value);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setJSONObjectDouble(JSONObject jsonObject, const char* key, double value, Exception* exception)
{
	try
	{
		static_cast<json::utility::jsonObject*>(jsonObject)->setDouble(key, value);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setJSONObjectBoolean(JSONObject jsonObject, const char* key, bool value, Exception* exception)
{
	try
	{
		static_cast<json::utility::jsonObject*>(jsonObject)->setBool(key, value);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setJSONObjectNull(JSONObject jsonObject, const char* key, Exception* exception)
{
	try
	{
		static_cast<json::utility::jsonObject*>(jsonObject)->setNull(key);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setJSONObjectArray(JSONObject jsonObject, const char* key, const JSONObject* objects, size_t size, Exception* exception)
{
	try
	{
		std::vector<json::utility::jsonObject> values;

		values.reserve(size);

		for (size_t i = 0; i < size; i++)
		{
			values.emplace_back(*static_cast<json::utility::jsonObject*>(objects[i]));
		}

		static_cast<json::utility::jsonObject*>(jsonObject)->setArray(key, std::move(values));
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

String buildJSONBuilder(JSONBuilder builder, Exception* exception)
{
	try
	{
		return new std::string(static_cast<json::JSONBuilder*>(builder)->build());
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void appendJSONBuilderObject(JSONBuilder builder, const char* key, JSONObject object, Exception* exception)
{
	try
	{
		static_cast<json::JSONBuilder*>(builder)->appendObject(key, *static_cast<json::utility::jsonObject*>(object));
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void appendJSONBuilderString(JSONBuilder builder, const char* key, const char* value, Exception* exception)
{
	try
	{
		static_cast<json::JSONBuilder*>(builder)->appendString(key, value);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void appendJSONBuilderInteger(JSONBuilder builder, const char* key, int64_t value, Exception* exception)
{
	try
	{
		static_cast<json::JSONBuilder*>(builder)->appendInt(key, value);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void appendJSONBuilderUnsignedInteger(JSONBuilder builder, const char* key, uint64_t value, Exception* exception)
{
	try
	{
		static_cast<json::JSONBuilder*>(builder)->appendUnsignedInt(key, value);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void appendJSONBuilderDouble(JSONBuilder builder, const char* key, double value, Exception* exception)
{
	try
	{
		static_cast<json::JSONBuilder*>(builder)->appendDouble(key, value);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void appendJSONBuilderBoolean(JSONBuilder builder, const char* key, bool value, Exception* exception)
{
	try
	{
		static_cast<json::JSONBuilder*>(builder)->appendBool(key, value);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void appendJSONBuilderNull(JSONBuilder builder, const char* key, Exception* exception)
{
	try
	{
		static_cast<json::JSONBuilder*>(builder)->appendNull(key);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void appendJSONBuilderArray(JSONBuilder builder, const char* key, const JSONObject* objects, size_t size, Exception* exception)
{
	try
	{
		std::vector<json::utility::jsonObject> values;

		values.reserve(size);

		for (size_t i = 0; i < size; i++)
		{
			values.emplace_back(*static_cast<json::utility::jsonObject*>(objects[i]));
		}

		static_cast<json::JSONBuilder*>(builder)->appendArray(key, std::move(values));
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

JSONObject getJSONParserObject(JSONParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		json::utility::jsonObject& object = const_cast<json::utility::jsonObject&>(static_cast<json::JSONParser*>(parser)->getObject(key, recursive));

		return createJSONObject(&object, exception);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

const char* getJSONParserString(JSONParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JSONParser*>(parser)->getString(key, recursive).data();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

int64_t getJSONParserInteger(JSONParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JSONParser*>(parser)->getInt(key, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return 0;
}

uint64_t getJSONParserUnsignedInteger(JSONParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JSONParser*>(parser)->getUnsignedInt(key, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return 0;
}

double getJSONParserDouble(JSONParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JSONParser*>(parser)->getDouble(key, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return 0.0;
}

bool getJSONParserBoolean(JSONParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JSONParser*>(parser)->getBool(key, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return false;
}

void getJSONParserNull(JSONParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		static_cast<json::JSONParser*>(parser)->getNull(key, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void getJSONParserArray(JSONParser parser, const char* key, void(*addArrayValue)(JSONObject object, void* array), void* array, bool recursive, Exception* exception)
{
	try
	{
		const std::vector<json::utility::jsonObject>& result = static_cast<json::JSONParser*>(parser)->getArray(key, recursive);

		for (const json::utility::jsonObject& object : result)
		{
			json::utility::jsonObject& temp = const_cast<json::utility::jsonObject&>(object);

			addArrayValue(&temp, array);
		}
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

bool tryGetJSONParserObject(JSONParser parser, const char* key, JSONObject* value, bool recursive, Exception* exception)
{
	try
	{
		json::utility::jsonObject result;

		if (static_cast<json::JSONParser*>(parser)->tryGetObject(key, result, recursive))
		{
			*value = &result;

			return true;
		}
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return false;
}

bool tryGetJSONParserString(JSONParser parser, const char* key, String* value, bool recursive, Exception* exception)
{
	try
	{
		std::string result;

		if (static_cast<json::JSONParser*>(parser)->tryGetString(key, result, recursive))
		{
			*value = new std::string(result);

			return true;
		}
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return false;
}

bool tryGetJSONParserInteger(JSONParser parser, const char* key, int64_t* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JSONParser*>(parser)->tryGetInt(key, *value, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return false;
}

bool tryGetJSONParserUnsignedInteger(JSONParser parser, const char* key, uint64_t* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JSONParser*>(parser)->tryGetUnsignedInt(key, *value, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return false;
}

bool tryGetJSONParserDouble(JSONParser parser, const char* key, double* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JSONParser*>(parser)->tryGetDouble(key, *value, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return false;
}

bool tryGetJSONParserBoolean(JSONParser parser, const char* key, bool* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JSONParser*>(parser)->tryGetBool(key, *value, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return false;
}

bool tryGetJSONParserNull(JSONParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JSONParser*>(parser)->tryGetNull(key, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return false;
}

bool tryGetJSONParserArray(JSONParser parser, const char* key, void(*addArrayValue)(JSONObject object, void* array), void* array, bool recursive, Exception* exception)
{
	try
	{
		std::vector<json::utility::jsonObject> result;

		if (static_cast<json::JSONParser*>(parser)->tryGetArray(key, result, recursive))
		{
			for (const json::utility::jsonObject& object : result)
			{
				json::utility::jsonObject& temp = const_cast<json::utility::jsonObject&>(object);

				addArrayValue(&temp, array);
			}

			return true;
		}
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return false;
}

const char* getJSONParserRawData(JSONParser parser, Exception* exception)
{
	try
	{
		return static_cast<json::JSONParser*>(parser)->getRawData().data();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

TableObject getOrCreateTable(DatabaseObject database, const char* tableName, const char* createTableQuery, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IDatabase*>(database)->getOrCreateTable(tableName, createTableQuery);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

TableObject getTable(DatabaseObject database, const char* tableName, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IDatabase*>(database)->get(tableName);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

bool containsTable(DatabaseObject database, const char* tableName, TableObject* table, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IDatabase*>(database)->contains(tableName, reinterpret_cast<framework::interfaces::ITable**>(table));
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return false;
}

const char* getDatabaseName(DatabaseObject database, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IDatabase*>(database)->getDatabaseName();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

const char* getDatabaseFileName(DatabaseObject database, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::IDatabase*>(database)->getDatabaseFileName();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

SQLResultObject executeQuery(TableObject table, const char* query, SQLValueObject* values, size_t size, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::ITable*>(table)->execute(query, static_cast<const framework::interfaces::ISQLValue**>(static_cast<void*>(values)), size);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void deleteSQLResult(TableObject table, SQLResultObject result, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::ITable*>(table)->deleteResult(static_cast<framework::interfaces::ISQLResult*>(result));
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

const char* getTableName(TableObject table, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::ITable*>(table)->getTableName();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

SQLValueObject createSQLValue(Exception* exception)
{
	try
	{
		return new framework::SQLValueImplementation();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void setSQLValueInt(SQLValueObject sqlValue, int64_t value, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::ISQLValue*>(sqlValue)->setInt(value);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setSQLValueDouble(SQLValueObject sqlValue, double value, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::ISQLValue*>(sqlValue)->setDouble(value);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setSQLValueString(SQLValueObject sqlValue, const char* value, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::ISQLValue*>(sqlValue)->setString(value);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setSQLValueBool(SQLValueObject sqlValue, bool value, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::ISQLValue*>(sqlValue)->setBool(value);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setSQLValueNull(SQLValueObject sqlValue, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::ISQLValue*>(sqlValue)->setNull();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

void setSQLValueBlob(SQLValueObject sqlValue, const uint8_t* value, size_t size, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::ISQLValue*>(sqlValue)->setBlob(value, size);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

int64_t getSQLValueInt(SQLValueObject sqlValue, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::ISQLValue*>(sqlValue)->getInt();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return 0;
}

double getSQLValueDouble(SQLValueObject sqlValue, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::ISQLValue*>(sqlValue)->getDouble();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return 0.0;
}

const char* getSQLValueString(SQLValueObject sqlValue, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::ISQLValue*>(sqlValue)->getString();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

bool getSQLValueBool(SQLValueObject sqlValue, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::ISQLValue*>(sqlValue)->getBool();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return false;
}

const uint8_t* getSQLValueBlob(SQLValueObject sqlValue, size_t* size, Exception* exception)
{
	try
	{
		return static_cast<framework::interfaces::ISQLValue*>(sqlValue)->getBlob(size);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

int getSQLValueType(SQLValueObject sqlValue, Exception* exception)
{
	try
	{
		return static_cast<int>(static_cast<framework::interfaces::ISQLValue*>(sqlValue)->getType());
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return 0;
}

size_t getSQLResultSize(SQLResultObject result, Exception* exception)
{
	try
	{
		return static_cast<int>(static_cast<framework::interfaces::ISQLResult*>(result)->size());
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return 0;
}

void iterateSQLResult(SQLResultObject result, void(*initBuffer)(size_t size, void* buffer), void(*callback)(const char** columnNames, const void** columnValues, size_t size, size_t index, void* buffer), void* buffer, Exception* exception)
{
	try
	{
		static_cast<framework::interfaces::ISQLResult*>(result)->iterate(initBuffer, callback, buffer);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String generateWebFrameworkUUID(Exception* exception)
{
	try
	{
		return new std::string(utility::uuid::generateUUID());
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void deleteWebFrameworkJSONObject(JSONObject object)
{
	delete static_cast<json::utility::jsonObject*>(object);
}

void deleteWebFrameworkJSONBuilder(JSONBuilder builder)
{
	delete static_cast<json::JSONBuilder*>(builder);
}

void deleteWebFrameworkJSONParser(JSONParser parser)
{
	delete static_cast<json::JSONParser*>(parser);
}

void deleteWebFrameworkSQLValue(SQLValueObject value)
{
	delete static_cast<framework::interfaces::ISQLValue*>(value);
}
