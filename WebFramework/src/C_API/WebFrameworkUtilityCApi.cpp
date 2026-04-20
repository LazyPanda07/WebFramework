#include "C_API/WebFrameworkUtilityCApi.h"

#include <Log.h>
#include <JsonBuilder.h>
#include <JsonParser.h>
#include <UUID.h>
#include <jwt-cpp/jwt.h>
#include <MapJsonIterator.h>

#include "Utility/JSONSettingsParser.h"
#include "DatabaseInterfaces/IDatabase.h"
#include "Databases/SQLValueImplementation.h"
#include "SHA256.h"
#include "Assets/SingleBinaryAsset.h"
#include "Framework/WebFrameworkConstants.h"
#include "Exceptions/AlreadyLoggedException.h"

#define LOG_EXCEPTION() if (Log::isValid()) { Log::error("Exception: {} in {} function", "C_API", e.what(), __func__); }
#define CREATE_EXCEPTION() *exception = new std::runtime_error(e.what())
#define LOG_AND_CREATE_EXCEPTION() LOG_EXCEPTION(); CREATE_EXCEPTION()
#define UNEXPECTED_EXCEPTION() if (Log::isValid()) { Log::error("Somethind went wrong", "C_API"); } *exception = new std::runtime_error(std::format("Something went wrong in file: {} on line: {}", __FILE__, __LINE__));

JsonObject createJsonObject(JsonObject object, Exception* exception)
{
	try
	{
		return object ?
			new json::JsonObject(*static_cast<json::JsonObject*>(object)) :
			new json::JsonObject();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

JsonBuilder createJsonBuilder(JsonBuilder builder, Exception* exception)
{
	try
	{
		return builder ?
			new json::JsonBuilder(*static_cast<json::JsonBuilder*>(builder)) :
			new json::JsonBuilder(CP_UTF8);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

JsonBuilder createJsonBuilderFromString(const char* jsonString, Exception* exception)
{
	try
	{
		return new json::JsonBuilder(json::JsonParser(jsonString).getParsedData(), CP_UTF8);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

JsonParser createJsonParser(JsonParser parser, Exception* exception)
{
	try
	{
		return parser ?
			new json::JsonParser(*static_cast<json::JsonParser*>(parser)) :
			new json::JsonParser();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

JsonParser createJsonParserFromObject(JsonObject object, Exception* exception)
{
	try
	{
		return new json::JsonParser(*static_cast<json::JsonObject*>(object));
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

JsonParser createJsonParserFromString(const char* jsonString, Exception* exception)
{
	try
	{
		return new json::JsonParser(jsonString);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

JsonObject accessIndexOperatorJsonObjectChecked(JsonObject jsonObject, size_t index, Exception* exception)
{
	try
	{
		return &(*static_cast<json::JsonObject*>(jsonObject)).at(index);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

JsonObject accessKeyOperatorJsonObjectChecked(JsonObject jsonObject, const char* key, Exception* exception)
{
	try
	{
		return const_cast<void*>(static_cast<const void*>(&(*static_cast<const json::JsonObject*>(jsonObject))[key]));
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

JsonObject getJsonObjectObject(JsonObject jsonObject, Exception* exception)
{
	try
	{
		json::JsonObject& object = const_cast<json::JsonObject&>(static_cast<json::JsonObject*>(jsonObject)->get<json::JsonObject>());

		return createJsonObject(&object, exception);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

const char* getJsonObjectString(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->get<std::string>().data();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

int64_t getJsonObjectInteger(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->get<int64_t>();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

uint64_t getJsonObjectUnsignedInteger(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->get<uint64_t>();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

double getJsonObjectDouble(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->get<double>();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool getJsonObjectBoolean(JsonObject jsonObject, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->get<bool>();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void getJsonObjectNull(JsonObject jsonObject, Exception* exception)
{
	try
	{
		static_cast<json::JsonObject*>(jsonObject)->get<std::nullptr_t>();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void getJsonObjectArray(JsonObject jsonObject, void(*addArrayValue)(JsonObject object, void* array), void* array, Exception* exception)
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonObjectObjectByKey(JsonObject jsonObject, const char* key, JsonObject* value, bool recursive, Exception* exception)
{
	try
	{
		json::JsonObject result;

		if (static_cast<json::JsonObject*>(jsonObject)->tryGet<json::JsonObject>(key, result, recursive))
		{
			*value = createJsonObject(&result, exception);

			return true;
		}
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonObjectStringByKey(JsonObject jsonObject, const char* key, String* value, bool recursive, Exception* exception)
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonObjectIntegerByKey(JsonObject jsonObject, const char* key, int64_t* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->tryGet(key, *value, recursive);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonObjectUnsignedIntegerByKey(JsonObject jsonObject, const char* key, uint64_t* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->tryGet(key, *value, recursive);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonObjectDoubleByKey(JsonObject jsonObject, const char* key, double* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->tryGet(key, *value, recursive);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonObjectBooleanByKey(JsonObject jsonObject, const char* key, bool* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->tryGet(key, *value, recursive);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonObjectNullByKey(JsonObject jsonObject, const char* key, bool recursive, Exception* exception)
{
	try
	{
		std::nullptr_t value;

		return static_cast<json::JsonObject*>(jsonObject)->tryGet(key, value, recursive);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonObjectArrayByKey(JsonObject jsonObject, const char* key, void(*addArrayValue)(JsonObject object, void* array), void* array, bool recursive, Exception* exception)
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonObjectObject(JsonObject jsonObject, JsonObject* value, Exception* exception)
{
	try
	{
		json::JsonObject result;

		if (static_cast<json::JsonObject*>(jsonObject)->tryGet(result))
		{
			*value = createJsonObject(&result, exception);

			return true;
		}
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonObjectString(JsonObject jsonObject, String* value, Exception* exception)
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonObjectInteger(JsonObject jsonObject, int64_t* value, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->tryGet(*value);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonObjectUnsignedInteger(JsonObject jsonObject, uint64_t* value, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->tryGet(*value);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonObjectDouble(JsonObject jsonObject, double* value, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->tryGet(*value);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonObjectBoolean(JsonObject jsonObject, bool* value, Exception* exception)
{
	try
	{
		return static_cast<json::JsonObject*>(jsonObject)->tryGet(*value);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonObjectNull(JsonObject jsonObject, Exception* exception)
{
	try
	{
		std::nullptr_t value;

		return static_cast<json::JsonObject*>(jsonObject)->tryGet(value);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonObjectArray(JsonObject jsonObject, void(*addArrayValue)(JsonObject object, void* array), void* array, Exception* exception)
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void setJsonObjectObject(JsonObject jsonObject, JsonObject value, Exception* exception)
{
	try
	{
		*static_cast<json::JsonObject*>(jsonObject) = *static_cast<json::JsonObject*>(value);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void setJsonObjectString(JsonObject jsonObject, const char* value, Exception* exception)
{
	try
	{
		*static_cast<json::JsonObject*>(jsonObject) = value;
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void setJsonObjectInteger(JsonObject jsonObject, int64_t value, Exception* exception)
{
	try
	{
		*static_cast<json::JsonObject*>(jsonObject) = value;
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void setJsonObjectUnsignedInteger(JsonObject jsonObject, uint64_t value, Exception* exception)
{
	try
	{
		*static_cast<json::JsonObject*>(jsonObject) = value;
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void setJsonObjectDouble(JsonObject jsonObject, double value, Exception* exception)
{
	try
	{
		*static_cast<json::JsonObject*>(jsonObject) = value;
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void setJsonObjectBoolean(JsonObject jsonObject, bool value, Exception* exception)
{
	try
	{
		*static_cast<json::JsonObject*>(jsonObject) = value;
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void setJsonObjectNull(JsonObject jsonObject, Exception* exception)
{
	try
	{
		*static_cast<json::JsonObject*>(jsonObject) = nullptr;
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void setJsonObjectArray(JsonObject jsonObject, JsonObject* objects, size_t size, Exception* exception)
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

String buildJsonBuilder(JsonBuilder builder, Exception* exception)
{
	try
	{
		return new std::string(static_cast<json::JsonBuilder*>(builder)->build());
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void appendJsonBuilderObject(JsonBuilder builder, const char* key, JsonObject object, Exception* exception)
{
	try
	{
		(*static_cast<json::JsonBuilder*>(builder))[key] = *static_cast<json::JsonObject*>(object);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void appendJsonBuilderString(JsonBuilder builder, const char* key, const char* value, Exception* exception)
{
	try
	{
		(*static_cast<json::JsonBuilder*>(builder))[key] = value;
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void appendJsonBuilderInteger(JsonBuilder builder, const char* key, int64_t value, Exception* exception)
{
	try
	{
		(*static_cast<json::JsonBuilder*>(builder))[key] = value;
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void appendJsonBuilderUnsignedInteger(JsonBuilder builder, const char* key, uint64_t value, Exception* exception)
{
	try
	{
		(*static_cast<json::JsonBuilder*>(builder))[key] = value;
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void appendJsonBuilderDouble(JsonBuilder builder, const char* key, double value, Exception* exception)
{
	try
	{
		(*static_cast<json::JsonBuilder*>(builder))[key] = value;
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void appendJsonBuilderBoolean(JsonBuilder builder, const char* key, bool value, Exception* exception)
{
	try
	{
		(*static_cast<json::JsonBuilder*>(builder))[key] = value;
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void appendJsonBuilderNull(JsonBuilder builder, const char* key, Exception* exception)
{
	try
	{
		(*static_cast<json::JsonBuilder*>(builder))[key] = nullptr;
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void appendJsonBuilderArray(JsonBuilder builder, const char* key, JsonObject* objects, size_t size, Exception* exception)
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

JsonObject getJsonParserObject(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		json::JsonObject& object = const_cast<json::JsonObject&>(static_cast<json::JsonParser*>(parser)->get<json::JsonObject>(key, recursive));

		return createJsonObject(&object, exception);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

const char* getJsonParserString(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->get<std::string>(key, recursive).data();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

int64_t getJsonParserInteger(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->get<int64_t>(key, recursive);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

uint64_t getJsonParserUnsignedInteger(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->get<uint64_t>(key, recursive);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

double getJsonParserDouble(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->get<double>(key, recursive);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool getJsonParserBoolean(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->get<bool>(key, recursive);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void getJsonParserNull(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		static_cast<json::JsonParser*>(parser)->get<std::nullptr_t>(key, recursive);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void getJsonParserArray(JsonParser parser, const char* key, void(*addArrayValue)(JsonObject object, void* array), void* array, bool recursive, Exception* exception)
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonParserObject(JsonParser parser, const char* key, JsonObject* value, bool recursive, Exception* exception)
{
	try
	{
		json::JsonObject result;

		if (static_cast<json::JsonParser*>(parser)->tryGet<json::JsonObject>(key, result, recursive))
		{
			*value = createJsonObject(&result, exception);

			return true;
		}
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonParserString(JsonParser parser, const char* key, String* value, bool recursive, Exception* exception)
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonParserInteger(JsonParser parser, const char* key, int64_t* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->tryGet<int64_t>(key, *value, recursive);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonParserUnsignedInteger(JsonParser parser, const char* key, uint64_t* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->tryGet<uint64_t>(key, *value, recursive);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonParserDouble(JsonParser parser, const char* key, double* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->tryGet<double>(key, *value, recursive);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonParserBoolean(JsonParser parser, const char* key, bool* value, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->tryGet<bool>(key, *value, recursive);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonParserNull(JsonParser parser, const char* key, bool recursive, Exception* exception)
{
	try
	{
		std::nullptr_t value;

		return static_cast<json::JsonParser*>(parser)->tryGet<std::nullptr_t>(key, value, recursive);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

bool tryGetJsonParserArray(JsonParser parser, const char* key, void(*addArrayValue)(JsonObject object, void* array), void* array, bool recursive, Exception* exception)
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

const char* getJsonParserRawData(JsonParser parser, Exception* exception)
{
	try
	{
		return static_cast<json::JsonParser*>(parser)->getRawData().data();
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

EXPORT void* getJsonParserParsedData(JsonParser parser, bool weak, Exception* exception)
{
	try
	{
		json::JsonObject& object = const_cast<json::JsonObject&>(static_cast<json::JsonParser*>(parser)->getParsedData());

		return weak ? &object : createJsonObject(&object, exception);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

String generateSha256(const char* data, size_t size, Exception* exception)
{
	try
	{
		encoding::SHA256 encoder;

		return new std::string
		(
			encoder
			(
				std::string_view(data, size)
			)
		);
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void generateBinaryAssetFile(const char* directoryPath, const char* outputPath, void(*progressCallback)(float progress, const char* assetPath, void* data), void* data, Exception* exception)
{
	try
	{
		std::ofstream stream(outputPath, std::ios::binary);

		if (!stream.is_open())
		{
			throw std::runtime_error(std::format(framework::logging::message::cantCreateFile, outputPath));
		}

		stream.close();

		framework::asset::SingleBinaryAsset::SingleBinaryAssetHeader header;
		std::vector<std::pair<std::filesystem::path, std::filesystem::path>> assets;
		size_t assetNamesSize = 0;

		for (const auto& it : std::filesystem::recursive_directory_iterator(directoryPath))
		{
			if (!std::filesystem::is_regular_file(it) || std::filesystem::absolute(it.path()) == std::filesystem::absolute(outputPath))
			{
				continue;
			}

			const auto& [assetRelativePath, assetPath] = assets.emplace_back(std::filesystem::relative(it.path(), directoryPath), it.path());
			std::string assetName = assetRelativePath.string();

			assetNamesSize += assetName.size();
			header.fileDataSize += std::filesystem::file_size(assetPath);
			header.startFileDataOffset += sizeof(decltype(assetName.size())) + assetName.size() + sizeof(framework::asset::SingleBinaryAsset::Asset::offset) + sizeof(framework::asset::SingleBinaryAsset::Asset::size); // size of asset name + name + offset + size
		}

		stream.open(outputPath, std::ios::binary);

		if (!stream.is_open())
		{
			throw std::runtime_error(std::format(framework::logging::message::cantOpenFile, outputPath));
		}

		stream.write(reinterpret_cast<const char*>(&header), sizeof(header));

		size_t accumulatedSize = 0;

		for (const auto& [assetRelativePath, assetPath] : assets)
		{
			constexpr size_t serializedFieldsNumber = 3;

			std::string assetName = assetRelativePath.string();
			size_t assetNameSize = assetName.size();
			uint64_t offset = accumulatedSize;
			uint64_t size = std::filesystem::file_size(assetPath);

			stream.write(reinterpret_cast<const char*>(&assetNameSize), sizeof(assetNameSize));
			stream.write(assetName.data(), assetNameSize);
			stream.write(reinterpret_cast<const char*>(&offset), sizeof(offset));
			stream.write(reinterpret_cast<const char*>(&size), sizeof(size));

			accumulatedSize += size;
		}

		constexpr std::size_t chunkSize = 10 * 1024 * 1024; // 10 MiB
		std::vector<char> buffer(chunkSize);

		for (size_t i = 0; i < assets.size(); i++)
		{
			const auto& [assetRelativePath, assetPath] = assets[i];
			std::ifstream in(assetPath, std::ios::binary);

			if (progressCallback)
			{
				progressCallback(static_cast<float>(i + 1) / assets.size(), assetRelativePath.string().data(), data);
			}

			while (in)
			{
				in.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));

				if (std::streamsize bytesRead = in.gcount())
				{
					stream.write(buffer.data(), bytesRead);

					if (!stream)
					{
						throw std::runtime_error("Failed to write to output stream");
					}
				}
			}

			if (!in.eof() && in.fail())
			{
				throw std::runtime_error(std::format("Failed to read from {} stream", assetPath.string()));
			}
		}
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

String createJwt(JsonObject data, int64_t expirationTimeInMinutes, Exception* exception)
{
	try
	{
		auto builder = jwt::create();
		const json::JsonObject& temp = *static_cast<json::JsonObject*>(data);

		if (!temp.is<json::JsonObject>())
		{
			throw std::runtime_error("JWT data must be JSON map");
		}

		json::MapJsonIterator it(temp);

		for (const auto& [key, value] : it)
		{
			if (value.is<std::string>())
			{
				jwt::traits::kazuho_picojson::value_type jsonValue(value.get<std::string>());

				builder.set_payload_claim
				(
					key,
					jsonValue
				);
			}
			else if (value.is<int64_t>())
			{
				jwt::traits::kazuho_picojson::value_type jsonValue(value.get<int64_t>());

				builder.set_payload_claim
				(
					key,
					jsonValue
				);
			}
			else if (value.is<bool>())
			{
				jwt::traits::kazuho_picojson::value_type jsonValue(value.get<bool>());

				builder.set_payload_claim
				(
					key,
					jsonValue
				);
			}
			else if (value.is<double>())
			{
				jwt::traits::kazuho_picojson::value_type jsonValue(value.get<double>());

				builder.set_payload_claim
				(
					key,
					jsonValue
				);
			}
		}

		builder.set_expires_in(std::chrono::minutes(expirationTimeInMinutes));

		return new std::string(builder.sign(jwt::algorithm::hs256("secret")));
	}
	catch (const framework::exceptions::AlreadyLoggedException& e)
	{
		CREATE_EXCEPTION();
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

void deleteWebFrameworkJsonObject(JsonObject object)
{
	delete static_cast<json::JsonObject*>(object);
}

void deleteWebFrameworkJsonBuilder(JsonBuilder builder)
{
	delete static_cast<json::JsonBuilder*>(builder);
}

void deleteWebFrameworkJsonParser(JsonParser parser)
{
	delete static_cast<json::JsonParser*>(parser);
}

void deleteWebFrameworkSQLValue(SqlValueObject value)
{
	delete static_cast<framework::interfaces::ISQLValue*>(value);
}
