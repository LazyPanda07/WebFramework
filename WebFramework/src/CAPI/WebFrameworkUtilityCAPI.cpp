#include "WebFrameworkUtilityCAPI.h"

#include "Log.h"
#include "JSONBuilder.h"
#include "JSONParser.h"

#define LOG_EXCEPTION() if (Log::isValid()) { Log::error("Exception: {}", "C_API", e.what()); }
#define CREATE_EXCEPTION() *exception = new std::runtime_error(e.what())
#define LOG_AND_CREATE_EXCEPTION() LOG_EXCEPTION(); CREATE_EXCEPTION()
#define UNEXPECTED_EXCEPTION() if (Log::isValid()) { Log::error("Somethind went wrong", "C_API"); } *exception = new std::runtime_error("Something went wrong");

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

void setObject(JSONObject jsonObject, const char* key, JSONObject object, Exception* exception)
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

void setString(JSONObject jsonObject, const char* key, const char* value, Exception* exception)
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

void setInteger(JSONObject jsonObject, const char* key, int64_t value, Exception* exception)
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

void setUnsignedInteger(JSONObject jsonObject, const char* key, uint64_t value, Exception* exception)
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

void setDouble(JSONObject jsonObject, const char* key, double value, Exception* exception)
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

void setBoolean(JSONObject jsonObject, const char* key, bool value, Exception* exception)
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

void setNull(JSONObject jsonObject, const char* key, Exception* exception)
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

void setArray(JSONObject jsonObject, const char* key, const JSONObject* objects, size_t size, Exception* exception)
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

String build(JSONBuilder builder, Exception* exception)
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

void appendObject(JSONBuilder builder, const char* key, JSONObject object, Exception* exception)
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

void appendString(JSONBuilder builder, const char* key, const char* value, Exception* exception)
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

void appendInteger(JSONBuilder builder, const char* key, int64_t value, Exception* exception)
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

void appendUnsignedInteger(JSONBuilder builder, const char* key, uint64_t value, Exception* exception)
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

void appendDouble(JSONBuilder builder, const char* key, double value, Exception* exception)
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

void appendBoolean(JSONBuilder builder, const char* key, bool value, Exception* exception)
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

void appendNull(JSONBuilder builder, const char* key, Exception* exception)
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

void appendArray(JSONBuilder builder, const char* key, const JSONObject* objects, size_t size, Exception* exception)
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

JSONObject getObject(JSONParser parser, const char* key, bool recursive, Exception* exception)
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

const char* getString(JSONParser parser, const char* key, bool recursive, Exception* exception)
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

int64_t getInteger(JSONParser parser, const char* key, bool recursive, Exception* exception)
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

uint64_t getUnsignedInteger(JSONParser parser, const char* key, bool recursive, Exception* exception)
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

double getDouble(JSONParser parser, const char* key, bool recursive, Exception* exception)
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

bool getBoolean(JSONParser parser, const char* key, bool recursive, Exception* exception)
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

void getNull(JSONParser parser, const char* key, bool recursive, Exception* exception)
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

void getArray(JSONParser parser, const char* key, void(*addArrayValue)(JSONObject object, void* array), void* array, bool recursive, Exception* exception)
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

bool tryGetObject(JSONParser parser, const char* key, JSONObject* value, bool recursive, Exception* exception)
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

bool tryGetString(JSONParser parser, const char* key, String* value, bool recursive, Exception* exception)
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

bool tryGetInteger(JSONParser parser, const char* key, int64_t* value, bool recursive, Exception* exception)
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

bool tryGetUnsignedInteger(JSONParser parser, const char* key, uint64_t* value, bool recursive, Exception* exception)
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

bool tryGetDouble(JSONParser parser, const char* key, double* value, bool recursive, Exception* exception)
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

bool tryGetBoolean(JSONParser parser, const char* key, bool* value, bool recursive, Exception* exception)
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

bool tryGetNull(JSONParser parser, const char* key, bool recursive, Exception* exception)
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

bool tryGetArray(JSONParser parser, const char* key, void(*addArrayValue)(JSONObject object, void* array), void* array, bool recursive, Exception* exception)
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void deleteWebFrameworkJSONObject(JSONObject object)
{
	delete static_cast<json::utility::jsonObject*>(object);
}

void deleteWebFrameworkJSONBuider(JSONBuilder builder)
{
	delete static_cast<json::JSONBuilder*>(builder);
}

void deleteWebFrameworkJSONParser(JSONParser parser)
{
	delete static_cast<json::JSONParser*>(parser);
}
