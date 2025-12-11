#include "HttpResponse.h"

WebFrameworkException setBody(HTTPResponse implementation, const char* body)
{
	WebFrameworkException exception = NULL;

	typedef void (*setResponseBody)(void* implementation, const char* body, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setResponseBody, body, &exception);

	return exception;
}

WebFrameworkException setJSONBody(HTTPResponse implementation, JSONBuilder body)
{
	WebFrameworkException exception = NULL;

	typedef void (*setResponseJSONBody)(void* implementation, void* body, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setResponseJSONBody, body, &exception);

	return exception;
}

WebFrameworkException setHTTPVersion(HTTPResponse implementation, const char* version)
{
	WebFrameworkException exception = NULL;

	typedef void (*setVersion)(void* implementation, const char* version, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setVersion, version, &exception);

	return exception;
}

WebFrameworkException setHTTPResponseCode(HTTPResponse implementation, ResponseCodes_t responseCode)
{
	WebFrameworkException exception = NULL;

	typedef void (*setResponseCode)(void* implementation, int64_t code, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setResponseCode, (int64_t)responseCode, &exception);

	return exception;
}

WebFrameworkException addHTTPResponseHeader(HTTPResponse implementation, const char* name, const char* value)
{
	WebFrameworkException exception = NULL;

	typedef void (*addResponseHeader)(void* implementation, const char* name, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(addResponseHeader, name, value, &exception);

	return exception;
}

WebFrameworkException appendHTTPResponseBody(HTTPResponse implementation, const char* body)
{
	WebFrameworkException exception = NULL;

	typedef void (*appendResponseBody)(void* implementation, const char* body, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendResponseBody, body, &exception);

	return exception;
}

WebFrameworkException addHTTPResponseCookie(HTTPResponse implementation, const char* name, const char* value)
{
	WebFrameworkException exception = NULL;

	typedef void (*addResponseCookie)(void* implementation, const char* name, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(addResponseCookie, name, value, &exception);

	return exception;
}

WebFrameworkException setHTTPResponseDefault(HTTPResponse implementation)
{
	WebFrameworkException exception = NULL;

	typedef void (*setResponseDefault)(void* implementation, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setResponseDefault, &exception);

	return exception;
}

WebFrameworkException setHTTPResponseIsValid(HTTPResponse implementation, bool isValid)
{
	WebFrameworkException exception = NULL;

	typedef void (*setResponseDefault)(void* implementation, bool isValid, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setResponseDefault, isValid, &exception);

	return exception;
}
