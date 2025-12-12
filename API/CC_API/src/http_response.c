#include "http_response.h"

web_framework_exception_t wf_set_body(http_response_t implementation, const char* body)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setResponseBody)(void* implementation, const char* body, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setResponseBody, body, &exception);

	return exception;
}

web_framework_exception_t wf_set_json_body(http_response_t implementation, json_builder_t body)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setResponseJsonBody)(void* implementation, void* body, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setResponseJsonBody, body, &exception);

	return exception;
}

web_framework_exception_t wf_set_http_version(http_response_t implementation, const char* version)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setVersion)(void* implementation, const char* version, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setVersion, version, &exception);

	return exception;
}

web_framework_exception_t wf_set_http_response_code(http_response_t implementation, response_codes_t responseCode)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setResponseCode)(void* implementation, int64_t code, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setResponseCode, (int64_t)responseCode, &exception);

	return exception;
}

web_framework_exception_t wf_add_http_response_header(http_response_t implementation, const char* name, const char* value)
{
	web_framework_exception_t exception = NULL;

	typedef void (*addResponseHeader)(void* implementation, const char* name, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(addResponseHeader, name, value, &exception);

	return exception;
}

web_framework_exception_t wf_append_http_response_body(http_response_t implementation, const char* body)
{
	web_framework_exception_t exception = NULL;

	typedef void (*appendResponseBody)(void* implementation, const char* body, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendResponseBody, body, &exception);

	return exception;
}

web_framework_exception_t wf_add_http_response_cookie(http_response_t implementation, const char* name, const char* value)
{
	web_framework_exception_t exception = NULL;

	typedef void (*addResponseCookie)(void* implementation, const char* name, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(addResponseCookie, name, value, &exception);

	return exception;
}

web_framework_exception_t wf_set_http_response_default(http_response_t implementation)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setResponseDefault)(void* implementation, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setResponseDefault, &exception);

	return exception;
}

web_framework_exception_t wf_set_http_response_is_valid(http_response_t implementation, bool isValid)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setResponseDefault)(void* implementation, bool isValid, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setResponseDefault, isValid, &exception);

	return exception;
}
