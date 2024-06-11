#pragma once

#ifdef WEB_FRAMEWORK_DLL

#ifdef __LINUX__
#define WEB_FRAMEWORK_API extern "C" __attribute__((visibility("default")))
#else
#define WEB_FRAMEWORK_API extern "C" __declspec(dllexport)
#endif

WEB_FRAMEWORK_API void deleteWebFrameworkObject(void* implementation);

WEB_FRAMEWORK_API void* createWebFrameworkFromPath(const char* configPath);

WEB_FRAMEWORK_API void* createWebFrameworkFromString(const char* serverConfiguration, const char* sourcesPath);

WEB_FRAMEWORK_API void startWebFrameworkServer(void* implementation, bool wait, void* onStartServer);

WEB_FRAMEWORK_API void stopWebFrameworkServer(void* implementation, bool wait);

#endif