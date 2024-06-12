#ifdef WEB_FRAMEWORK_DLL

#include "web_framework_c_api.h"

#include "WebFramework.h"
#include "Log.h"

void deleteWebFrameworkObject(void* implementation)
{
	delete implementation;
}

void* createWebFrameworkFromPath(const char* configPath, void** exception)
{
	try
	{
		return new framework::WebFramework(std::filesystem::path(configPath));
	}
	catch (const std::exception& e)
	{
		Log::error("Exception: {}", "C_API", e.what());

		*exception = new std::runtime_error(e.what());
	}

	return nullptr;
}

void* createWebFrameworkFromString(const char* serverConfiguration, const char* sourcesPath, void** exception)
{
	try
	{
		return new framework::WebFramework(framework::utility::Config::createConfig(serverConfiguration, sourcesPath));
	}
	catch (const std::exception& e)
	{
		Log::error("Exception: {}", "C_API", e.what());

		*exception = new std::runtime_error(e.what());
	}

	return nullptr;
}

void startWebFrameworkServerCXX(void* implementation, bool wait, void* onStartServer, void** exception)
{
	try
	{
		std::function<void()>* lambda = static_cast<std::function<void()>*>(onStartServer);

		static_cast<framework::WebFramework*>(implementation)->start(wait, *lambda);
	}
	catch (const std::exception& e)
	{
		Log::error("Exception: {}", "C_API", e.what());

		*exception = new std::runtime_error(e.what());
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
		Log::error("Exception: {}", "C_API", e.what());

		*exception = new std::runtime_error(e.what());
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
		Log::error("Exception: {}", "C_API", e.what());

		*exception = new std::runtime_error(e.what());
	}
}

const char* getErrorMessage(void* exception)
{
	return static_cast<std::runtime_error*>(exception)->what();
}

#endif
