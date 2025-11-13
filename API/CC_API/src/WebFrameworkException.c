#include "WebFrameworkException.h"

const char* getErrorMessage(WebFrameworkException exception)
{
    if (exception)
    {
        return getDataFromString(exception);
    }

    return NULL;
}
