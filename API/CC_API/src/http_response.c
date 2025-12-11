#include "HttpResponse.h"

web_framework_exception_t setBody(http_response_t implementation, const char* body)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setResponseBody)(void* implementation, const char* body, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setResponseBody, body, &exception);

	return exception;
}

web_framework_exception_t setJsonBody(http_response_t implementation, json_builder_t body)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setResponseJsonBody)(void* implementation, void* body, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setResponseJsonBody, body, &exception);

	return exception;
}

web_framework_exception_t setHTTPVersion(http_response_t implementation, const char* version)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setVersion)(void* implementation, const char* version, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setVersion, version, &exception);

	return exception;
}

web_framework_exception_t setHTTPResponseCode(http_response_t implementation, ResponseCodes_t responseCode)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setResponseCode)(void* implementation, int64_t code, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setResponseCode, (int64_t)responseCode, &exception);

	return exception;
}

web_framework_exception_t addHTTPResponseHeader(http_response_t implementation, const char* name, const char* value)
{
	web_framework_exception_t exception = NULL;

	typedef void (*addResponseHeader)(void* implementation, const char* name, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(addResponseHeader, name, value, &exception);

	return exception;
}

web_framework_exception_t appendHTTPResponseBody(http_response_t implementation, const char* body)
{
	web_framework_exception_t exception = NULL;

	typedef void (*appendResponseBody)(void* implementation, const char* body, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendResponseBody, body, &exception);

	return exception;
}

web_framework_exception_t addHTTPResponseCookie(http_response_t implementation, const char* name, const char* value)
{
	web_framework_exception_t exception = NULL;

	typedef void (*addResponseCookie)(void* implementation, const char* name, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(addResponseCookie, name, value, &exception);

	return exception;
}

web_framework_exception_t setHTTPResponseDefault(http_response_t implementation)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setResponseDefault)(void* implementation, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setResponseDefault, &exception);

	return exception;
}

web_framework_exception_t setHTTPResponseIsValid(http_response_t implementation, bool isValid)
{
	web_framework_exception_t exception = NULL;

	typedef void (*setResponseDefault)(void* implementation, bool isValid, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setResponseDefault, isValid, &exception);

	return exception;
}
