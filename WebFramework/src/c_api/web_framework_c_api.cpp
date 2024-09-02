#ifdef WEB_FRAMEWORK_DLL

#include "web_framework_c_api.h"

#include "Import/WebFramework.h"
#include "Import/Config.h"
#include "Log.h"

#define LOG_EXCEPTION() if (Log::isValid()) { Log::error("Exception: {}", "C_API", e.what()); }
#define CREATE_EXCEPTION() *exception = new std::runtime_error(e.what())
#define LOG_AND_CREATE_EXCEPTION() LOG_EXCEPTION(); CREATE_EXCEPTION()

void deleteWebFrameworkObject(void* implementation)
{
	delete implementation;
}

const char* getDataFromString(void* implementation)
{
	return static_cast<std::string*>(implementation)->data();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void* createWebFrameworkFromPath(const char* configPath, void** exception)
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

void* createWebFrameworkFromString(const char* serverConfiguration, const char* applicationDirectory, void** exception)
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

void* createWebFrameworkFromConfig(void* config, void** exception)
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

void* createConfigFromPath(const char* configPath, void** exception)
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

void* createConfigFromString(const char* serverConfiguration, const char* applicationDirectory, void** exception)
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void startWebFrameworkServerCXX(void* implementation, bool wait, void* onStartServer, void** exception)
{
	try
	{
		std::function<void()>* lambda = static_cast<std::function<void()>*>(onStartServer);

		static_cast<framework::WebFramework*>(implementation)->start(wait, *lambda);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

void startWebFrameworkServer(void* implementation, bool wait, void (*onStartServer)(), void** exception)
{
	try
	{
		static_cast<framework::WebFramework*>(implementation)->start(wait, onStartServer);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

void stopWebFrameworkServer(void* implementation, bool wait, void** exception)
{
	try
	{
		static_cast<framework::WebFramework*>(implementation)->stop(wait);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

void overrideConfigurationString(void* implementation, const char* key, const char* value, bool recursive, void** exception)
{
	try
	{
		static_cast<framework::utility::Config*>(implementation)->overrideConfiguration(key, std::string(value), recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

void overrideConfigurationInteger(void* implementation, const char* key, int64_t value, bool recursive, void** exception)
{
	try
	{
		static_cast<framework::utility::Config*>(implementation)->overrideConfiguration(key, value, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

void overrideConfigurationBoolean(void* implementation, const char* key, bool value, bool recursive, void** exception)
{
	try
	{
		static_cast<framework::utility::Config*>(implementation)->overrideConfiguration(key, value, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

void overrideConfigurationStringArray(void* implementation, const char* key, const char** value, bool recursive, int64_t size, void** exception)
{
	try
	{
		std::vector<json::utility::jsonObject> data;

		data.reserve(size);

		for (int64_t i = 0; i < size; i++)
		{
			json::utility::appendArray(std::string(value[i]), data);
		}

		static_cast<framework::utility::Config*>(implementation)->overrideConfiguration(key, data, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

void overrideConfigurationIntegerArray(void* implementation, const char* key, const int64_t* value, bool recursive, int64_t size, void** exception)
{
	try
	{
		std::vector<json::utility::jsonObject> data;

		data.reserve(size);

		for (int64_t i = 0; i < size; i++)
		{
			json::utility::appendArray(value[i], data);
		}

		static_cast<framework::utility::Config*>(implementation)->overrideConfiguration(key, data, recursive);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

void overrideBasePath(void* implementation, const char* basePath, void** exception)
{
	try
	{
		reinterpret_cast<framework::utility::Config*>(implementation)->overrideBasePath(basePath);
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
}

void* getConfiguration(void* implementation, void** exception)
{
	try
	{
		framework::utility::Config& config = *static_cast<framework::utility::Config*>(implementation);
		std::ostringstream result;

		result << *config;

		return new std::string(result.str());
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}

	return nullptr;
}

const char* getRawConfiguration(void* implementation, void** exception)
{
	try
	{
		return (*(*static_cast<framework::utility::Config*>(implementation))).getRawData().data();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}

	return nullptr;
}

void* getBasePath(void* implementation, void** exception)
{
	try
	{
		return new std::string(static_cast<framework::utility::Config*>(implementation)->getBasePath().string());
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}

	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* getErrorMessage(void* exception)
{
	return static_cast<std::runtime_error*>(exception)->what();
}

#endif
