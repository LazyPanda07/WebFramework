#pragma once

#include "DLLHandler.h"

/**
 * @brief Get error message from exception or NULL if exception is NULL
 * @param exception WebFrameworkException
 * @return Error message or NULL if exception is NULL
 */
const char* getErrorMessage(WebFrameworkException exception);

inline const char* getErrorMessage(WebFrameworkException exception)
{
    if (exception)
    {
        return getDataFromString(exception);
    }

    return NULL;
}
