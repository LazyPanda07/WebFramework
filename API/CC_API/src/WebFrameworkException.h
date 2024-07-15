#pragma once

#include "DLLHandler.h"

typedef void* WebFrameworkException;

const char* getErrorMessage(WebFrameworkException exception);

inline const char* getErrorMessage(WebFrameworkException exception)
{
    return getDataFromString(exception);
}
