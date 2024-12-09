#pragma once

#include "DLLHandler.h"
#include "Config.h"
#include "WebFrameworkException.h"

/**
 * @brief Create WebFramework
 * @param configPath Path to *.json config
 * @param server Result WebFramework
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException createWebFrameworkFromPath(const char* configPath, WebFramework* server);

/**
 * @brief Create WebFramework
 * @param serverConfiguration *.json config file content 
 * @param applicationDirectory Working directory
 * @param server Result WebFramework
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException createWebFrameworkFromString(const char* serverConfiguration, const char* applicationDirectory, WebFramework* server);

/**
 * @brief Create WebFramework
 * @param config Config from one of createConfig functions
 * @param server Result WebFramework
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException createWebFrameworkFromConfig(Config config, WebFramework* server);

/**
 * @brief Start server
 * @param implementation WebFramework
 * @param wait Wait until server stop
 * @param onStartServer On start server callback
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException startWebFrameworkServer(WebFramework implementation, bool wait, void (*onStartServer)());

/**
 * @brief Stop server
 * @param implementation WebFramework
 * @param wait Wait until server stop
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException stopWebFrameworkServer(WebFramework implementation, bool wait);

/**
 * @brief Is server running
 * @param implementation WebFramework
 * @param isServerRunning Result
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
WebFrameworkException isServerRunning(WebFramework implementation, bool* result);

/**
 * @brief Get WebFramework version
 * @return String representation of version in format {major}.{minor}.{patch}
 */
const char* getWebFrameworkVersion();

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

inline WebFrameworkException isServerRunning(WebFramework implementation, bool* result)
{
    WebFrameworkException exception = NULL;

    typedef bool (*isServerRunning)(void* implementation, void** exception);

    *result = CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(isServerRunning, &exception);

    return exception;
}

inline const char* getWebFrameworkVersion()
{
    typedef const char* (*getWebFrameworkVersion)();

    return CALL_WEB_FRAMEWORK_FUNCTION(getWebFrameworkVersion);
}
