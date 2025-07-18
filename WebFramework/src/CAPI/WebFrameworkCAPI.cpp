#include "WebFrameworkCAPI.h"

#include <fstream>

#include "Framework/WebFramework.h"
#include "Framework/Config.h"
#include "Log.h"
#include "WebNetwork/Interfaces/IHTTPRequest.h"
#include "Exceptions/APIException.h"

#define LOG_EXCEPTION() if (Log::isValid()) { Log::error("Exception: {}", "C_API", e.what()); }
#define CREATE_EXCEPTION() *exception = new std::runtime_error(e.what())
#define LOG_AND_CREATE_EXCEPTION() LOG_EXCEPTION(); CREATE_EXCEPTION()
#define UNEXPECTED_EXCEPTION() if (Log::isValid()) { Log::error("Somethind went wrong", "C_API"); } *exception = new std::runtime_error("Something went wrong"); std::ofstream("error.txt") << "Something went wrong"

const char* getDataFromString(String string)
{
	return static_cast<std::string*>(string)->data();
}

const char* getErrorMessage(Exception exception)
{
	return static_cast<std::runtime_error*>(exception)->what();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
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
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

void throwException(void* httpRequest, const char* errorMessage, int64_t responseCode, const char* logCategory, size_t exceptionClassHash)
{
	static_cast<framework::interfaces::IHTTPRequest*>(httpRequest)->throwException(errorMessage, responseCode, logCategory, exceptionClassHash);
}

bool checkExceptionHash(const Exception exception, size_t hash)
{
	const std::exception* temp = static_cast<const std::exception*>(exception);

	if (const framework::exceptions::APIException* apiException = dynamic_cast<const framework::exceptions::APIException*>(temp))
	{
		return apiException->getExceptionClassHash() == hash;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
