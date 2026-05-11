#include "web_framework_exception.h"

const char* wf_get_error_message(web_framework_exception_t exception)
{
    if (exception)
    {
        typedef const char* (*getErrorMessage)(void* exception);

        return CALL_WEB_FRAMEWORK_FUNCTION(getErrorMessage, exception);
    }

    return NULL;
}
