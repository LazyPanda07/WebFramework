#pragma once

#include "DLLHandler.h"

/**
 * @brief Get error message from exception or NULL if exception is NULL
 * @param exception WebFrameworkException
 * @return Error message or NULL if exception is NULL
 */
const char* getErrorMessage(WebFrameworkException exception);

#ifndef __WEB_FRAMEWORK_WEB_FRAMEWORK_EXCEPTION_IMPLEMENTATION__
#define __WEB_FRAMEWORK_WEB_FRAMEWORK_EXCEPTION_IMPLEMENTATION__

const char* getErrorMessage(WebFrameworkException exception)
{
    if (exception)
    {
        return getDataFromString(exception);
    }

    return NULL;
}
#endif
