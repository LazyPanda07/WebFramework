#ifdef WEB_FRAMEWORK_DLL

#include "web_framework_c_api.h"

#include "WebFramework.h"

void deleteWebFrameworkObject(void* implementation)
{
	delete implementation;
}

void* createWebFrameworkFromPath(const char* configPath)
{
	return new framework::WebFramework(std::filesystem::path(configPath));
}

void* createWebFrameworkFromString(const char* serverConfiguration, const char* sourcesPath)
{
	return new framework::WebFramework(framework::utility::Config::createConfig(serverConfiguration, sourcesPath));
}

void startWebFrameworkServer(void* implementation, bool wait, void* onStartServer)
{
	std::function<void()>* pointer = static_cast<std::function<void()>*>(onStartServer);

	static_cast<framework::WebFramework*>(implementation)->start(wait, *pointer);
}

void stopWebFrameworkServer(void* implementation, bool wait)
{
	static_cast<framework::WebFramework*>(implementation)->stop(wait);
}

#endif
