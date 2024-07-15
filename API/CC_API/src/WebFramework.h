#pragma once

#include "DLLHandler.h"
#include "Config.h"
#include "WebFrameworkException.h"

typedef void* WebFramework;

WebFrameworkException createWebFrameworkFromPath(const char* configPath, WebFramework* server);

WebFrameworkException createWebFrameworkFromString(const char* serverConfiguration, const char* applicationDirectory, WebFramework* server);

WebFrameworkException createWebFrameworkFromConfig(Config config, WebFramework* server);

WebFrameworkException startWebFrameworkServer(WebFramework implementation, bool wait, void (*onStartServer)());

WebFrameworkException stopWebFrameworkServer(WebFramework implementation, bool wait);

inline WebFrameworkException createWebFrameworkFromPath(const char* configPath, WebFramework* server)
{
    WebFrameworkException exception = NULL;

    typedef void* (*createWebFrameworkFromPath)(const char* configPath, void** exception);

    *server = CALL_WEB_FRAMEWORK_FUNCTION(createWebFrameworkFromPath, configPath, &exception);

    return exception;
}

inline WebFrameworkException createWebFrameworkFromString(const char* serverConfiguration, const char* applicationDirectory, WebFramework* server)
{
    WebFrameworkException exception = NULL;

    typedef void* (*createWebFrameworkFromString)(const char* serverConfiguration, const char* applicationDirectory, void** exception);

    *server = CALL_WEB_FRAMEWORK_FUNCTION(createWebFrameworkFromString, serverConfiguration, applicationDirectory, &exception);

    return exception;
}

inline WebFrameworkException createWebFrameworkFromConfig(Config config, WebFramework* server)
{
    WebFrameworkException exception = NULL;

    typedef void* (*createWebFrameworkFromConfig)(void* config, void** exception);

    *server = CALL_WEB_FRAMEWORK_FUNCTION(createWebFrameworkFromConfig, config, &exception);

    return exception;
}

inline WebFrameworkException startWebFrameworkServer(WebFramework implementation, bool wait, void (*onStartServer)())
{
    WebFrameworkException exception = NULL;

    typedef void (*startWebFrameworkServer)(void* implementation, bool wait, void (*onStartServer)(), void** exception);

    CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(startWebFrameworkServer, wait, onStartServer, &exception);

    return exception;
}

inline WebFrameworkException stopWebFrameworkServer(WebFramework implementation, bool wait)
{
    WebFrameworkException exception = NULL;

    typedef void (*stopWebFrameworkServer)(void* implementation, bool wait, void** exception);

    CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(stopWebFrameworkServer, wait, &exception);

    return exception;
}
