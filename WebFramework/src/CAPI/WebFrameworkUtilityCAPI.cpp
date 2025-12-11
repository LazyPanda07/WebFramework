#include "WebFrameworkUtilityCAPI.h"

#include <Log.h>
#include <JsonBuilder.h>
#include <JsonParser.h>
#include <UUID.h>

#include "Utility/JSONSettingsParser.h"
#include "DatabaseInterfaces/IDatabase.h"
#include "Databases/SQLValueImplementation.h"

#define LOG_EXCEPTION() if (Log::isValid()) { Log::error("Exception: {} in {} function", "C_API", e.what(), __func__); }
#define CREATE_EXCEPTION() *exception = new std::runtime_error(e.what())
#define LOG_AND_CREATE_EXCEPTION() LOG_EXCEPTION(); CREATE_EXCEPTION()
#define UNEXPECTED_EXCEPTION() if (Log::isValid()) { Log::error("Somethind went wrong", "C_API"); } *exception = new std::runtime_error(std::format("Something went wrong in file: {} on line: {}", __FILE__, __LINE__));

JsonObject createJSONObject(JsonObject object, Exception* exception)
{
	try
	{
		return object ?
			new json::JsonObject(*static_cast<json::JsonObject*>(object)) :
			new json::JsonObject();
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

JsonBuilder createJSONBuilder(JsonBuilder builder, Exception* exception)
{
	try
	{
		return builder ?
			new json::JsonBuilder(*static_cast<json::JsonBuilder*>(builder)) :
			new json::JsonBuilder(CP_UTF8);
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

JsonBuilder createJSONBuilderFromString(const char* jsonString, Exception* exception)
{
	try
	{
		return new json::JsonBuilder(json::JsonParser(jsonString).getParsedData(), CP_UTF8);
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

JsonParser createJSONParser(JsonParser parser, Exception* exception)
{
	try
	{
		return parser ?
			new json::JsonParser(*static_cast<json::JsonParser*>(parser)) :
			new json::JsonParser();
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

JsonParser createJSONParserFromString(const char* jsonString, Exception* exception)
{
	try
	{
		return new json::JsonParser(jsonString);
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

JsonObject accessIndexOperatorJsonObject(JsonObject jsonObject, size_t index, Exception* exception)
{
	try
	{
		return &(*static_cast<json::JsonObject*>(jsonObject))[index];
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

JsonObject accessKeyOperatorJsonObject(JsonObject jsonObject, const char* key, Exception* exception)
{
	try
	{
		return &(*static_cast<json::JsonObject*>(jsonObject))[key];
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

JsonObject emplaceBackObject(JsonObject jsonObject, JsonObject value, Exception* exception)
{
	try
	{
		return &static_cast<json::JsonObject*>(jsonObject)->emplace_back(*static_cast<json::JsonObject*>(value));
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

JsonObject emplaceBackString(JsonObject jsonObject, const char* value, Exception* exception)
{
	try
	{
		return &static_cast<json::JsonObject*>(jsonObject)->emplace_back(std::string_view(value));
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

JsonObject emplaceBackInteger(JsonObject jsonObject, int64_t value, Exception* exception)
{
	try
	{
		return &static_cast<json::JsonObject*>(jsonObject)->emplace_back(value);
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

JsonObject emplaceBackUnsignedInteger(JsonObject jsonObject, uint64_t value, Exception* exception)
{
	try
	{
		return &static_cast<json::JsonObject*>(jsonObject)->emplace_back(value);
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

JsonObject emplaceBackDouble(JsonObject jsonObject, double value, Exception* exception)
{
	try
	{
		return &static_cast<json::JsonObject*>(jsonObject)->emplace_back(value);
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

JsonObject emplaceBackBoolean(JsonObject jsonObject, bool value, Exception* exception)
{
	try
	{
		return &static_cast<json::JsonObject*>(jsonObject)->emplace_back(value);
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

JsonObject emplaceBackNull(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return &static_cast<json::JsonObject*>(jsonObject)->emplace_back(nullptr);
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

JsonObject emplaceBackArray(JsonObject jsonObject, JsonObject* objects, size_t size, Exception* exception)
{
	try
	{
		std::vector<json::JsonObject> array;

		array.reserve(size);

		for (size_t i = 0; i < size; i++)
		{
			array.emplace_back(json::JsonObject(*static_cast<json::JsonObject*>(objects[i])));
		}

		return &static_cast<json::JsonObject*>(jsonObject)->emplace_back(std::move(array));
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

bool isObject(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->is<json::JsonObject>();
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

bool isString(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->is<std::string>();
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

bool isInteger(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->is<int64_t>();
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

bool isUnsignedInteger(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->is<uint64_t>();
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

bool isDouble(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->is<double>();
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

bool isBoolean(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->is<bool>();
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

bool isNull(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->is<std::nullptr_t>();
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

bool isArray(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->is<std::vector<json::JsonObject>>();
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

bool containsJsonObjectObject(JsonObject jsonObject, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->contains<json::JsonObject>(key, recursive);
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

bool containsJsonObjectString(JsonObject jsonObject, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->contains<std::string>(key, recursive);
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

bool containsJsonObjectInteger(JsonObject jsonObject, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->contains<int64_t>(key, recursive);
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

bool containsJsonObjectUnsignedInteger(JsonObject jsonObject, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->contains<uint64_t>(key, recursive);
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

bool containsJsonObjectDouble(JsonObject jsonObject, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->contains<double>(key, recursive);
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

bool containsJsonObjectBoolean(JsonObject jsonObject, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->contains<bool>(key, recursive);
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

bool containsJsonObjectNull(JsonObject jsonObject, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->contains<std::nullptr_t>(key, recursive);
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

bool containsJsonObjectArray(JsonObject jsonObject, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->contains<std::vector<json::JsonObject>>(key, recursive);
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

size_t sizeJsonObject(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->size();
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

JsonObject getJSONObjectObject(JsonObject jsonObject, Exception* exception)
{
	try
	{
		json::JsonObject& object = const_cast<json::JsonObject&>(static_cast<json::JsonObject*>(jsonObject)->get<json::JsonObject>());

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

const char* getJSONObjectString(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->get<std::string>().data();
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

int64_t getJSONObjectInteger(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->get<int64_t>();
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

uint64_t getJSONObjectUnsignedInteger(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->get<uint64_t>();
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

double getJSONObjectDouble(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->get<double>();
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

bool getJSONObjectBoolean(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->get<bool>();
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

void getJSONObjectNull(JsonObject jsonObject, Exception* exception)
{
	try
	{
		static_cast<json::JsonObject*>(jsonObject)->get<std::nullptr_t>();
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

void getJSONObjectArray(JsonObject jsonObject, void(*addArrayValue)(JsonObject object, void* array), void* array, Exception* exception)
{
	try
	{
		const std::vector<json::JsonObject>& result = static_cast<json::JsonObject*>(jsonObject)->get<std::vector<json::JsonObject>>();

		for (const json::JsonObject& object : result)
		{
			json::JsonObject& temp = const_cast<json::JsonObject&>(object);

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

bool tryGetJSONObjectObjectByKey(JsonObject jsonObject, const char* key, JsonObject* value, bool recursive, Exception* exception)
{
	try
	{
		json::JsonObject result;

		if (static_cast<json::JsonObject*>(jsonObject)->tryGet<json::JsonObject>(key, result, recursive))
		{
			*value = createJSONObject(&result, exception);

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

bool tryGetJSONObjectStringByKey(JsonObject jsonObject, const char* key, String* value, bool recursive, Exception* exception)
{
	try
	{
		std::string result;

		if (static_cast<json::JsonObject*>(jsonObject)->tryGet<std::string>(key, result, recursive))
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

bool tryGetJSONObjectIntegerByKey(JsonObject jsonObject, const char* key, int64_t* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->tryGet(key, *value, recursive);
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

bool tryGetJSONObjectUnsignedIntegerByKey(JsonObject jsonObject, const char* key, uint64_t* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->tryGet(key, *value, recursive);
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

bool tryGetJSONObjectDoubleByKey(JsonObject jsonObject, const char* key, double* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->tryGet(key, *value, recursive);
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

bool tryGetJSONObjectBooleanByKey(JsonObject jsonObject, const char* key, bool* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->tryGet(key, *value, recursive);
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

bool tryGetJSONObjectNullByKey(JsonObject jsonObject, const char* key, bool recursive, Exception* exception)
{
	try
	{
		std::nullptr_t value;

		return static_cast<json::JsonObject*>(jsonObject)->tryGet(key, value, recursive);
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

bool tryGetJSONObjectArrayByKey(JsonObject jsonObject, const char* key, void(*addArrayValue)(JsonObject object, void* array), void* array, bool recursive, Exception* exception)
{
	try
	{
		std::vector<json::JsonObject> result;

		if (static_cast<json::JsonObject*>(jsonObject)->tryGet<std::vector<json::JsonObject>>(key, result, recursive))
		{
			for (const json::JsonObject& object : result)
			{
				json::JsonObject& temp = const_cast<json::JsonObject&>(object);

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

bool tryGetJSONObjectObject(JsonObject jsonObject, JsonObject* value, Exception* exception)
{
	try
	{
		json::JsonObject result;

		if (static_cast<json::JsonObject*>(jsonObject)->tryGet(result))
		{
			*value = createJSONObject(&result, exception);

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

bool tryGetJSONObjectString(JsonObject jsonObject, String* value, Exception* exception)
{
	try
	{
		std::string result;

		if (static_cast<json::JsonObject*>(jsonObject)->tryGet<std::string>(result))
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

bool tryGetJSONObjectInteger(JsonObject jsonObject, int64_t* value, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->tryGet(*value);
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

bool tryGetJSONObjectUnsignedInteger(JsonObject jsonObject, uint64_t* value, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->tryGet(*value);
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

bool tryGetJSONObjectDouble(JsonObject jsonObject, double* value, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->tryGet(*value);
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

bool tryGetJSONObjectBoolean(JsonObject jsonObject, bool* value, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->tryGet(*value);
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

bool tryGetJSONObjectNull(JsonObject jsonObject, Exception* exception)
{
	try
	{
		std::nullptr_t value;

		return static_cast<json::JsonObject*>(jsonObject)->tryGet(value);
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

bool tryGetJSONObjectArray(JsonObject jsonObject, void(*addArrayValue)(JsonObject object, void* array), void* array, Exception* exception)
{
	try
	{
		std::vector<json::JsonObject> result;

		if (static_cast<json::JsonObject*>(jsonObject)->tryGet<std::vector<json::JsonObject>>(result))
		{
			for (const json::JsonObject& object : result)
			{
				json::JsonObject& temp = const_cast<json::JsonObject&>(object);

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

void setJSONObjectObject(JsonObject jsonObject, JsonObject value, Exception* exception)
{
	try
	{
		*static_cast<json::JsonObject*>(jsonObject) = *static_cast<json::JsonObject*>(value);
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

void setJSONObjectString(JsonObject jsonObject, const char* value, Exception* exception)
{
	try
	{
		*static_cast<json::JsonObject*>(jsonObject) = value;
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

void setJSONObjectInteger(JsonObject jsonObject, int64_t value, Exception* exception)
{
	try
	{
		*static_cast<json::JsonObject*>(jsonObject) = value;
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

void setJSONObjectUnsignedInteger(JsonObject jsonObject, uint64_t value, Exception* exception)
{
	try
	{
		*static_cast<json::JsonObject*>(jsonObject) = value;
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

void setJSONObjectDouble(JsonObject jsonObject, double value, Exception* exception)
{
	try
	{
		*static_cast<json::JsonObject*>(jsonObject) = value;
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

void setJSONObjectBoolean(JsonObject jsonObject, bool value, Exception* exception)
{
	try
	{
		*static_cast<json::JsonObject*>(jsonObject) = value;
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

void setJSONObjectNull(JsonObject jsonObject, Exception* exception)
{
	try
	{
		*static_cast<json::JsonObject*>(jsonObject) = nullptr;
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

void setJSONObjectArray(JsonObject jsonObject, JsonObject* objects, size_t size, Exception* exception)
{
	try
	{
		std::vector<json::JsonObject> values;

		values.reserve(size);

		for (size_t i = 0; i < size; i++)
		{
			values.emplace_back(*static_cast<json::JsonObject*>(objects[i]));
		}

		*static_cast<json::JsonObject*>(jsonObject) = std::move(values);
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

String jsonObjectToString(JsonObject jsonObject, Exception* exception)
{
	try
	{
		std::ostringstream stream;

		stream << *static_cast<json::JsonObject*>(jsonObject);

		return new std::string(stream.str());
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

String buildJSONBuilder(JsonBuilder builder, Exception* exception)
{
	try
	{
		return new std::string(static_cast<json::JsonBuilder*>(builder)->build());
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

void standardJsonBuilder(JsonBuilder builder, Exception* exception)
{
	try
	{
		static_cast<json::JsonBuilder*>(builder)->standard();
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

void minimizeJsonBuilder(JsonBuilder builder, Exception* exception)
{
	try
	{
		static_cast<json::JsonBuilder*>(builder)->minimize();
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

bool containsJsonBuilderObject(JsonBuilder builder, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonBuilder*>(builder)->contains<json::JsonObject>(key, recursive);
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

bool containsJsonBuilderString(JsonBuilder builder, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonBuilder*>(builder)->contains<std::string>(key, recursive);
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

bool containsJsonBuilderInteger(JsonBuilder builder, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonBuilder*>(builder)->contains<int64_t>(key, recursive);
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

bool containsJsonBuilderUnsignedInteger(JsonBuilder builder, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonBuilder*>(builder)->contains<uint64_t>(key, recursive);
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

bool containsJsonBuilderDouble(JsonBuilder builder, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonBuilder*>(builder)->contains<double>(key, recursive);
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

bool containsJsonBuilderBoolean(JsonBuilder builder, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonBuilder*>(builder)->contains<bool>(key, recursive);
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

bool containsJsonBuilderNull(JsonBuilder builder, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonBuilder*>(builder)->contains<std::nullptr_t>(key, recursive);
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

bool containsJsonBuilderArray(JsonBuilder builder, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonBuilder*>(builder)->contains<std::vector<json::JsonObject>>(key, recursive);
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

void appendJSONBuilderObject(JsonBuilder builder, const char* key, JsonObject object, Exception* exception)
{
	try
	{
		(*static_cast<json::JsonBuilder*>(builder))[key] = *static_cast<json::JsonObject*>(object);
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

void appendJSONBuilderString(JsonBuilder builder, const char* key, const char* value, Exception* exception)
{
	try
	{
		(*static_cast<json::JsonBuilder*>(builder))[key] = value;
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

void appendJSONBuilderInteger(JsonBuilder builder, const char* key, int64_t value, Exception* exception)
{
	try
	{
		(*static_cast<json::JsonBuilder*>(builder))[key] = value;
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

void appendJSONBuilderUnsignedInteger(JsonBuilder builder, const char* key, uint64_t value, Exception* exception)
{
	try
	{
		(*static_cast<json::JsonBuilder*>(builder))[key] = value;
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

void appendJSONBuilderDouble(JsonBuilder builder, const char* key, double value, Exception* exception)
{
	try
	{
		(*static_cast<json::JsonBuilder*>(builder))[key] = value;
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

void appendJSONBuilderBoolean(JsonBuilder builder, const char* key, bool value, Exception* exception)
{
	try
	{
		(*static_cast<json::JsonBuilder*>(builder))[key] = value;
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

void appendJSONBuilderNull(JsonBuilder builder, const char* key, Exception* exception)
{
	try
	{
		(*static_cast<json::JsonBuilder*>(builder))[key] = nullptr;
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

void appendJSONBuilderArray(JsonBuilder builder, const char* key, JsonObject* objects, size_t size, Exception* exception)
{
	try
	{
		std::vector<json::JsonObject> values;

		values.reserve(size);

		for (size_t i = 0; i < size; i++)
		{
			values.emplace_back(*static_cast<json::JsonObject*>(objects[i]));
		}

		(*static_cast<json::JsonBuilder*>(builder))[key] = std::move(values);
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

JsonObject accessKeyOperatorJsonBuilder(JsonBuilder builder, const char* key, Exception* exception)
{
	try
	{
		return &(*static_cast<json::JsonBuilder*>(builder))[key];
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

bool containsJsonParserObject(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->contains<json::JsonObject>(key, recursive);
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

bool containsJsonParserString(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->contains<std::string>(key, recursive);
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

bool containsJsonParserInteger(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->contains<int64_t>(key, recursive);
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

bool containsJsonParserUnsignedInteger(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->contains<uint64_t>(key, recursive);
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

bool containsJsonParserDouble(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->contains<double>(key, recursive);
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

bool containsJsonParserBoolean(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->contains<bool>(key, recursive);
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

bool containsJsonParserNull(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->contains<std::nullptr_t>(key, recursive);
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

bool containsJsonParserArray(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->contains<std::vector<json::JsonObject>>(key, recursive);
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

void overrideJsonParserObject(JsonParser parser, const char* key, JsonObject value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->overrideValue(key, *static_cast<json::JsonObject*>(value), recursive);
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

void overrideJsonParserString(JsonParser parser, const char* key, const char* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->overrideValue(key, std::string_view(value), recursive);
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

void overrideJsonParserInteger(JsonParser parser, const char* key, int64_t value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->overrideValue(key, value, recursive);
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

void overrideJsonParserUnsignedInteger(JsonParser parser, const char* key, uint64_t value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->overrideValue(key, value, recursive);
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

void overrideJsonParserDouble(JsonParser parser, const char* key, double value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->overrideValue(key, value, recursive);
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

void overrideJsonParserBoolean(JsonParser parser, const char* key, bool value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->overrideValue(key, value, recursive);
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

void overrideJsonParserNull(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->overrideValue(key, nullptr, recursive);
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

void overrideJsonParserArray(JsonParser parser, const char* key, JsonObject* value, size_t size, bool recursive, Exception* exception)
{
	try
	{
		std::vector<json::JsonObject> values;

		values.reserve(size);

		for (size_t i = 0; i < size; i++)
		{
			values.emplace_back(*static_cast<json::JsonObject*>(value[i]));
		}

		static_cast<json::JsonParser*>(parser)->overrideValue(key, std::move(values), recursive);
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

JsonObject getJSONParserObject(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		json::JsonObject& object = const_cast<json::JsonObject&>(static_cast<json::JsonParser*>(parser)->get<json::JsonObject>(key, recursive));

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

const char* getJSONParserString(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->get<std::string>(key, recursive).data();
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

int64_t getJSONParserInteger(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->get<int64_t>(key, recursive);
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

uint64_t getJSONParserUnsignedInteger(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->get<uint64_t>(key, recursive);
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

double getJSONParserDouble(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->get<double>(key, recursive);
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

bool getJSONParserBoolean(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->get<bool>(key, recursive);
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

void getJSONParserNull(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		static_cast<json::JsonParser*>(parser)->get<std::nullptr_t>(key, recursive);
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

void getJSONParserArray(JsonParser parser, const char* key, void(*addArrayValue)(JsonObject object, void* array), void* array, bool recursive, Exception* exception)
{
	try
	{
		const std::vector<json::JsonObject>& result = static_cast<json::JsonParser*>(parser)->get<std::vector<json::JsonObject>>(key, recursive);

		for (const json::JsonObject& object : result)
		{
			json::JsonObject& temp = const_cast<json::JsonObject&>(object);

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

bool tryGetJSONParserObject(JsonParser parser, const char* key, JsonObject* value, bool recursive, Exception* exception)
{
	try
	{
		json::JsonObject result;

		if (static_cast<json::JsonParser*>(parser)->tryGet<json::JsonObject>(key, result, recursive))
		{
			*value = createJSONObject(&result, exception);

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

bool tryGetJSONParserString(JsonParser parser, const char* key, String* value, bool recursive, Exception* exception)
{
	try
	{
		std::string result;

		if (static_cast<json::JsonParser*>(parser)->tryGet<std::string>(key, result, recursive))
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

bool tryGetJSONParserInteger(JsonParser parser, const char* key, int64_t* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->tryGet<int64_t>(key, *value, recursive);
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

bool tryGetJSONParserUnsignedInteger(JsonParser parser, const char* key, uint64_t* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->tryGet<uint64_t>(key, *value, recursive);
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

bool tryGetJSONParserDouble(JsonParser parser, const char* key, double* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->tryGet<double>(key, *value, recursive);
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

bool tryGetJSONParserBoolean(JsonParser parser, const char* key, bool* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->tryGet<bool>(key, *value, recursive);
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

bool tryGetJSONParserNull(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		std::nullptr_t value;

		return static_cast<json::JsonParser*>(parser)->tryGet<std::nullptr_t>(key, value, recursive);
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

bool tryGetJSONParserArray(JsonParser parser, const char* key, void(*addArrayValue)(JsonObject object, void* array), void* array, bool recursive, Exception* exception)
{
	try
	{
		std::vector<json::JsonObject> result;

		if (static_cast<json::JsonParser*>(parser)->tryGet<std::vector<json::JsonObject>>(key, result, recursive))
		{
			for (const json::JsonObject& object : result)
			{
				json::JsonObject& temp = const_cast<json::JsonObject&>(object);

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

const char* getJSONParserRawData(JsonParser parser, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->getRawData().data();
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

SqlResultObject executeQuery(TableObject table, const char* query, SqlValueObject* values, size_t size, Exception* exception)
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

void deleteSQLResult(TableObject table, SqlResultObject result, Exception* exception)
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

SqlValueObject createSQLValue(Exception* exception)
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

void setSQLValueInt(SqlValueObject sqlValue, int64_t value, Exception* exception)
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

void setSQLValueDouble(SqlValueObject sqlValue, double value, Exception* exception)
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

void setSQLValueString(SqlValueObject sqlValue, const char* value, Exception* exception)
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

void setSQLValueBool(SqlValueObject sqlValue, bool value, Exception* exception)
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

void setSQLValueNull(SqlValueObject sqlValue, Exception* exception)
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

void setSQLValueBlob(SqlValueObject sqlValue, const uint8_t* value, size_t size, Exception* exception)
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

int64_t getSQLValueInt(SqlValueObject sqlValue, Exception* exception)
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

double getSQLValueDouble(SqlValueObject sqlValue, Exception* exception)
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

const char* getSQLValueString(SqlValueObject sqlValue, Exception* exception)
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

bool getSQLValueBool(SqlValueObject sqlValue, Exception* exception)
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

const uint8_t* getSQLValueBlob(SqlValueObject sqlValue, size_t* size, Exception* exception)
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

int getSQLValueType(SqlValueObject sqlValue, Exception* exception)
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

size_t getSQLResultSize(SqlResultObject result, Exception* exception)
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

void iterateSQLResult(SqlResultObject result, void(*initBuffer)(size_t size, void* buffer), void(*callback)(const char** columnNames, const void** columnValues, size_t size, size_t index, void* buffer), void* buffer, Exception* exception)
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

void deleteWebFrameworkJSONObject(JsonObject object)
{
	delete static_cast<json::JsonObject*>(object);
}

void deleteWebFrameworkJSONBuilder(JsonBuilder builder)
{
	delete static_cast<json::JsonBuilder*>(builder);
}

void deleteWebFrameworkJSONParser(JsonParser parser)
{
	delete static_cast<json::JsonParser*>(parser);
}

void deleteWebFrameworkSQLValue(SqlValueObject value)
{
	delete static_cast<framework::interfaces::ISQLValue*>(value);
}
