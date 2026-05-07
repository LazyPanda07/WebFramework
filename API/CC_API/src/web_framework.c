#include "web_framework.h"

web_framework_exception_t wf_create_web_framework_from_path(const char* configPath, web_framework_t* server)
{
    web_framework_exception_t exception = NULL;

    typedef void* (*createWebFrameworkFromPath)(const char* configPath, void** exception);

    server->implementation = CALL_WEB_FRAMEWORK_FUNCTION(createWebFrameworkFromPath, configPath, &exception);
    server->weak = false;

    return exception;
}

web_framework_exception_t wf_create_web_framework_from_string(const char* serverConfiguration, const char* applicationDirectory, web_framework_t* server)
{
    web_framework_exception_t exception = NULL;

    typedef void* (*createWebFrameworkFromString)(const char* serverConfiguration, const char* applicationDirectory, void** exception);

    server->implementation = CALL_WEB_FRAMEWORK_FUNCTION(createWebFrameworkFromString, serverConfiguration, applicationDirectory, &exception);
    server->weak = false;

    return exception;
}

web_framework_exception_t wf_create_web_framework_from_config(config_t config, web_framework_t* server)
{
    web_framework_exception_t exception = NULL;

    typedef void* (*createWebFrameworkFromConfig)(void* config, void** exception);

    server->implementation = CALL_WEB_FRAMEWORK_FUNCTION(createWebFrameworkFromConfig, config, &exception);
    server->weak = false;

    return exception;
}

web_framework_exception_t wf_start_web_framework_server(web_framework_t web_framework, bool wait, void (*onStartServer)())
{
    web_framework_exception_t exception = NULL;
    void* implementation = web_framework.implementation;

    typedef void (*startWebFrameworkServer)(void* implementation, bool wait, void (*onStartServer)(), void** exception);

    CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(startWebFrameworkServer, wait, onStartServer, &exception);

    return exception;
}

web_framework_exception_t wf_stop_web_framework_server(web_framework_t web_framework, bool wait)
{
    web_framework_exception_t exception = NULL;
    void* implementation = web_framework.implementation;

    typedef void (*stopWebFrameworkServer)(void* implementation, bool wait, void** exception);

    CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(stopWebFrameworkServer, wait, &exception);

    return exception;
}

web_framework_exception_t wf_kick_web_framework_server(web_framework_t web_framework, const char* ip)
{
    web_framework_exception_t exception = NULL;
    void* implementation = web_framework.implementation;

    typedef void (*kickWebFrameworkServer)(void* implementation, const char* ip, void** exception);

    CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(kickWebFrameworkServer, ip, &exception);

    return exception;
}

web_framework_exception_t wf_update_ssl_certificates_web_framework_server(web_framework_t web_framework)
{
    web_framework_exception_t exception = NULL;
    void* implementation = web_framework.implementation;

    typedef void (*updateSslCertificatesWebFrameworkServer)(void* implementation, void** exception);

    CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(updateSslCertificatesWebFrameworkServer, &exception);

    return exception;
}

web_framework_exception_t wf_is_server_running(web_framework_t web_framework, bool* result)
{
    web_framework_exception_t exception = NULL;
    void* implementation = web_framework.implementation;

    typedef bool (*isServerRunning)(void* implementation, void** exception);

    *result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(isServerRunning, &exception);

    return exception;
}

const char* wf_get_web_framework_version()
{
    typedef const char* (*getWebFrameworkVersion)();

    return CALL_WEB_FRAMEWORK_FUNCTION(getWebFrameworkVersion);
}
