#include "utility/web_framework_utility.h"

web_framework_exception_t wf_generate_uuid(web_framework_string_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*generateWebFrameworkUUID)(void** exception);

	*result = CALL_WEB_FRAMEWORK_FUNCTION(generateWebFrameworkUUID, &exception);

	return exception;
}

web_framework_exception_t wf_generate_sha256(const char* data, size_t size, web_framework_string_t* result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*generateSha256)(const char* data, size_t size, void** exception);

	*result = CALL_WEB_FRAMEWORK_FUNCTION(generateSha256, data, size, &exception);

	return exception;
}
