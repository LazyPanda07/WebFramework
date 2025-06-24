#include "web_framework_c_api.h"

#include "Framework/WebFramework.h"
#include "Framework/Config.h"
#include "Log.h"

#define LOG_EXCEPTION() if (Log::isValid()) { Log::error("Exception: {}", "C_API", e.what()); }
#define CREATE_EXCEPTION() *exception = new std::runtime_error(e.what())
#define LOG_AND_CREATE_EXCEPTION() LOG_EXCEPTION(); CREATE_EXCEPTION()

void deleteWebFrameworkString(String string)
{
	delete static_cast<std::string*>(string);
}

void deleteWebFrameworkConfig(Config config)
{
	delete static_cast<framework::utility::Config*>(config);
}

void deleteWebFramework(WebFramework webFramework)
{
	delete static_cast<framework::WebFramework*>(webFramework);
}

void deleteWebFrameworkException(Exception exception)
{
	delete static_cast<std::runtime_error*>(exception);
}

void deleteWebFrameworkJSONBuider(JSONBuilder builder)
{
	delete static_cast<json::JSONBuilder*>(builder);
}

void deleteWebFrameworkJSONObject(JSONObject object)
{
	delete static_cast<json::utility::jsonObject*>(object);
}

const char* getDataFromString(String string)
{
	return static_cast<std::string*>(string)->data();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WebFramework createWebFrameworkFromPath(const char* configPath, Exception* exception)
{
	try
	{
		return new framework::WebFramework(std::filesystem::path(configPath));
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}

	return nullptr;
}

WebFramework createWebFrameworkFromString(const char* serverConfiguration, const char* applicationDirectory, Exception* exception)
{
	try
	{
		return new framework::WebFramework(framework::utility::Config(serverConfiguration, applicationDirectory));
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}

	return nullptr;
}

WebFramework createWebFrameworkFromConfig(Config config, Exception* exception)
{
	try
	{
		return new framework::WebFramework(*static_cast<framework::utility::Config*>(config));
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}

	return nullptr;
}

Config createConfigFromPath(const char* configPath, Exception* exception)
{
	try
	{
		return new framework::utility::Config(configPath);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}

	return nullptr;
}

Config createConfigFromString(const char* serverConfiguration, const char* applicationDirectory, Exception* exception)
{
	try
	{
		return new framework::utility::Config(serverConfiguration, applicationDirectory);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
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

	return nullptr;
}

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

	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void startWebFrameworkServerCXX(WebFramework server, bool wait, void* onStartServer, Exception* exception)
{
	try
	{
		std::function<void()>* lambda = static_cast<std::function<void()>*>(onStartServer);

		static_cast<framework::WebFramework*>(server)->start(wait, *lambda);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

void startWebFrameworkServer(WebFramework server, bool wait, void (*onStartServer)(), Exception* exception)
{
	try
	{
		static_cast<framework::WebFramework*>(server)->start(wait, onStartServer);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

void stopWebFrameworkServer(WebFramework server, bool wait, Exception* exception)
{
	try
	{
		static_cast<framework::WebFramework*>(server)->stop(wait);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

bool isServerRunning(WebFramework server, Exception* exception)
{
	try
	{
		return static_cast<framework::WebFramework*>(server)->isServerRunning();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}

	return false;
}

const char* getWebFrameworkVersion()
{
	constexpr std::string_view version = "3.0.12";

	return version.data();
}

void overrideConfigurationString(Config config, const char* key, const char* value, bool recursive, Exception* exception)
{
	try
	{
		static_cast<framework::utility::Config*>(config)->overrideConfiguration(key, std::string(value), recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

void overrideConfigurationInteger(Config config, const char* key, int64_t value, bool recursive, Exception* exception)
{
	try
	{
		static_cast<framework::utility::Config*>(config)->overrideConfiguration(key, value, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

void overrideConfigurationBoolean(Config config, const char* key, bool value, bool recursive, Exception* exception)
{
	try
	{
		static_cast<framework::utility::Config*>(config)->overrideConfiguration(key, static_cast<json::utility::jsonObject::variantType>(value), recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

void overrideConfigurationStringArray(Config config, const char* key, const char** value, bool recursive, int64_t size, Exception* exception)
{
	try
	{
		std::vector<json::utility::jsonObject> data;

		data.reserve(size);

		for (int64_t i = 0; i < size; i++)
		{
			json::utility::appendArray(std::string(value[i]), data);
		}

		static_cast<framework::utility::Config*>(config)->overrideConfiguration(key, data, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

void overrideConfigurationIntegerArray(Config config, const char* key, const int64_t* value, bool recursive, int64_t size, Exception* exception)
{
	try
	{
		std::vector<json::utility::jsonObject> data;

		data.reserve(size);

		for (int64_t i = 0; i < size; i++)
		{
			json::utility::appendArray(value[i], data);
		}

		static_cast<framework::utility::Config*>(config)->overrideConfiguration(key, data, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

String getConfigurationString(Config config, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return new std::string(static_cast<framework::utility::Config*>(config)->getConfigurationString(key, recursive));
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}

	return nullptr;
}

int64_t getConfigurationInteger(Config config, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<framework::utility::Config*>(config)->getConfigurationInteger(key, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}

	return -1;
}

bool getConfigurationBoolean(Config config, const char* key, bool recursive, Exception* exception)
{
	try
	{
		return static_cast<framework::utility::Config*>(config)->getConfigurationBoolean(key, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}

	return false;
}

void overrideBasePath(Config config, const char* basePath, Exception* exception)
{
	try
	{
		reinterpret_cast<framework::utility::Config*>(config)->overrideBasePath(basePath);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

void* getConfiguration(Config config, Exception* exception)
{
	try
	{
		std::ostringstream result;

		result << **static_cast<framework::utility::Config*>(config);

		return new std::string(result.str());
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}

	return nullptr;
}

const char* getRawConfiguration(Config config, Exception* exception)
{
	try
	{
		return (*(*static_cast<framework::utility::Config*>(config))).getRawData().data();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}

	return nullptr;
}

String getBasePath(Config config, Exception* exception)
{
	try
	{
		return new std::string(static_cast<framework::utility::Config*>(config)->getBasePath().string());
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}

	return nullptr;
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
}

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
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* getErrorMessage(Exception exception)
{
	return static_cast<std::runtime_error*>(exception)->what();
}
