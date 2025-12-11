#include "utility/web_framework_utility.h"

web_framework_exception_t wf_generate_uuid(web_framework_string_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*generateWebFrameworkUUID)(void** exception);

	*result = CALL_WEB_FRAMEWORK_FUNCTION(generateWebFrameworkUUID, &exception);

	return exception;
}
