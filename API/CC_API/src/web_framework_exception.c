#include "web_framework_exception_t.h"

const char* getErrorMessage(web_framework_exception_t exception)
{
    if (exception)
    {
        return getDataFromString(exception);
    }

    return NULL;
}
