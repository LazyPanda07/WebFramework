#include "web_framework.h"

web_framework_exception_t wf_create_web_framework_from_path(const char* configPath, web_framework_t* server)
{
    web_framework_exception_t exception = NULL;

    typedef void* (*createWebFrameworkFromPath)(const char* configPath, void** exception);

    *server = CALL_WEB_FRAMEWORK_FUNCTION(createWebFrameworkFromPath, configPath, &exception);

    return exception;
}

web_framework_exception_t wf_create_web_framework_from_string(const char* serverConfiguration, const char* applicationDirectory, web_framework_t* server)
{
    web_framework_exception_t exception = NULL;

    typedef void* (*createWebFrameworkFromString)(const char* serverConfiguration, const char* applicationDirectory, void** exception);

    *server = CALL_WEB_FRAMEWORK_FUNCTION(createWebFrameworkFromString, serverConfiguration, applicationDirectory, &exception);

    return exception;
}

web_framework_exception_t wf_create_web_framework_from_config(config_t config, web_framework_t* server)
{
    web_framework_exception_t exception = NULL;

    typedef void* (*createWebFrameworkFromConfig)(void* config, void** exception);

    *server = CALL_WEB_FRAMEWORK_FUNCTION(createWebFrameworkFromConfig, config, &exception);

    return exception;
}

web_framework_exception_t wf_start_web_framework_server(web_framework_t implementation, bool wait, void (*onStartServer)())
{
    web_framework_exception_t exception = NULL;

    typedef void (*startWebFrameworkServer)(void* implementation, bool wait, void (*onStartServer)(), void** exception);

    CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(startWebFrameworkServer, wait, onStartServer, &exception);

    return exception;
}

web_framework_exception_t wf_stop_web_framework_server(web_framework_t implementation, bool wait)
{
    web_framework_exception_t exception = NULL;

    typedef void (*stopWebFrameworkServer)(void* implementation, bool wait, void** exception);

    CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(stopWebFrameworkServer, wait, &exception);

    return exception;
}

web_framework_exception_t wf_is_server_running(web_framework_t implementation, bool* result)
{
    web_framework_exception_t exception = NULL;

    typedef bool (*isServerRunning)(void* implementation, void** exception);

    *result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(isServerRunning, &exception);

    return exception;
}

const char* wf_get_web_framework_version()
{
    typedef const char* (*getWebFrameworkVersion)();

    return CALL_WEB_FRAMEWORK_FUNCTION(getWebFrameworkVersion);
}
