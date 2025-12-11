#include "WebFramework.h"

WebFrameworkException createWebFrameworkFromPath(const char* configPath, WebFramework* server)
{
    WebFrameworkException exception = NULL;

    typedef void* (*createWebFrameworkFromPath)(const char* configPath, void** exception);

    *server = CALL_WEB_FRAMEWORK_FUNCTION(createWebFrameworkFromPath, configPath, &exception);

    return exception;
}

WebFrameworkException createWebFrameworkFromString(const char* serverConfiguration, const char* applicationDirectory, WebFramework* server)
{
    WebFrameworkException exception = NULL;

    typedef void* (*createWebFrameworkFromString)(const char* serverConfiguration, const char* applicationDirectory, void** exception);

    *server = CALL_WEB_FRAMEWORK_FUNCTION(createWebFrameworkFromString, serverConfiguration, applicationDirectory, &exception);

    return exception;
}

WebFrameworkException createWebFrameworkFromConfig(Config config, WebFramework* server)
{
    WebFrameworkException exception = NULL;

    typedef void* (*createWebFrameworkFromConfig)(void* config, void** exception);

    *server = CALL_WEB_FRAMEWORK_FUNCTION(createWebFrameworkFromConfig, config, &exception);

    return exception;
}

WebFrameworkException startWebFrameworkServer(WebFramework implementation, bool wait, void (*onStartServer)())
{
    WebFrameworkException exception = NULL;

    typedef void (*startWebFrameworkServer)(void* implementation, bool wait, void (*onStartServer)(), void** exception);

    CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(startWebFrameworkServer, wait, onStartServer, &exception);

    return exception;
}

WebFrameworkException stopWebFrameworkServer(WebFramework implementation, bool wait)
{
    WebFrameworkException exception = NULL;

    typedef void (*stopWebFrameworkServer)(void* implementation, bool wait, void** exception);

    CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(stopWebFrameworkServer, wait, &exception);

    return exception;
}

WebFrameworkException isServerRunning(WebFramework implementation, bool* result)
{
    WebFrameworkException exception = NULL;

    typedef bool (*isServerRunning)(void* implementation, void** exception);

    *result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(isServerRunning, &exception);

    return exception;
}

const char* getWebFrameworkVersion()
{
    typedef const char* (*getWebFrameworkVersion)();

    return CALL_WEB_FRAMEWORK_FUNCTION(getWebFrameworkVersion);
}
