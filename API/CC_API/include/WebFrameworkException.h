#pragma once

#include "DLLHandler.h"

/**
 * @brief Get error message from exception or NULL if exception is NULL
 * @param exception WebFrameworkException
 * @return Error message or NULL if exception is NULL
 */
const char* getErrorMessage(WebFrameworkException exception);
