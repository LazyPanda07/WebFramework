#include "web_framework_exception.h"

const char* wf_get_error_message(web_framework_exception_t exception)
{
    if (exception)
    {
        return wf_get_data_from_string(exception);
    }

    return NULL;
}
