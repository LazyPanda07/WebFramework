#pragma once

#include "DLLHandler.h"
#include "config_t.h"
#include "web_framework_exception_t.h"

/**
 * @brief Create web_framework_t
 * @param configPath Path to *.json config
 * @param server Result web_framework_t
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t createWebFrameworkFromPath(const char* configPath, web_framework_t* server);

/**
 * @brief Create web_framework_t
 * @param serverConfiguration *.json config file content 
 * @param applicationDirectory Working directory
 * @param server Result web_framework_t
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t createWebFrameworkFromString(const char* serverConfiguration, const char* applicationDirectory, web_framework_t* server);

/**
 * @brief Create web_framework_t
 * @param config config_t from one of createConfig functions
 * @param server Result web_framework_t
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t createWebFrameworkFromConfig(config_t config, web_framework_t* server);

/**
 * @brief Start server
 * @param implementation web_framework_t
 * @param wait Wait until server stop
 * @param onStartServer On start server callback
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t startWebFrameworkServer(web_framework_t implementation, bool wait, void (*onStartServer)());

/**
 * @brief Stop server
 * @param implementation web_framework_t
 * @param wait Wait until server stop
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t stopWebFrameworkServer(web_framework_t implementation, bool wait);

/**
 * @brief Is server running
 * @param implementation web_framework_t
 * @param isServerRunning Result
 * @return NULL if no errors. Call getErrorMessage for getting error message
 */
web_framework_exception_t isServerRunning(web_framework_t implementation, bool* result);

/**
 * @brief Get web_framework_t version
 * @return String representation of version in format {major}.{minor}.{patch}
 */
const char* getWebFrameworkVersion();
