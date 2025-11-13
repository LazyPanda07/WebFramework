#include "HttpResponse.h"

WebFrameworkException setBody(HTTPResponse implementation, const char* body)
{
	WebFrameworkException exception = NULL;

	typedef void (*setHTTPResponseBody)(void* implementation, const char* body, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setHTTPResponseBody, body, &exception);

	return exception;
}

WebFrameworkException setJSONBody(HTTPResponse implementation, JSONBuilder body)
{
	WebFrameworkException exception = NULL;

	typedef void (*setHTTPResponseJSONBody)(void* implementation, void* body, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setHTTPResponseJSONBody, body, &exception);

	return exception;
}

WebFrameworkException setHTTPVersion(HTTPResponse implementation, const char* version)
{
	WebFrameworkException exception = NULL;

	typedef void (*setHTTPVersion)(void* implementation, const char* version, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setHTTPVersion, version, &exception);

	return exception;
}

WebFrameworkException setHTTPResponseCode(HTTPResponse implementation, ResponseCodes_t responseCode)
{
	WebFrameworkException exception = NULL;

	typedef void (*setHTTPResponseCode)(void* implementation, int64_t code, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setHTTPResponseCode, (int64_t)responseCode, &exception);

	return exception;
}

WebFrameworkException addHTTPResponseHeader(HTTPResponse implementation, const char* name, const char* value)
{
	WebFrameworkException exception = NULL;

	typedef void (*addHTTPResponseHeader)(void* implementation, const char* name, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(addHTTPResponseHeader, name, value, &exception);

	return exception;
}

WebFrameworkException appendHTTPResponseBody(HTTPResponse implementation, const char* body)
{
	WebFrameworkException exception = NULL;

	typedef void (*appendHTTPResponseBody)(void* implementation, const char* body, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(appendHTTPResponseBody, body, &exception);

	return exception;
}

WebFrameworkException addHTTPResponseCookie(HTTPResponse implementation, const char* name, const char* value)
{
	WebFrameworkException exception = NULL;

	typedef void (*addHTTPResponseCookie)(void* implementation, const char* name, const char* value, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(addHTTPResponseCookie, name, value, &exception);

	return exception;
}

WebFrameworkException setHTTPResponseDefault(HTTPResponse implementation)
{
	WebFrameworkException exception = NULL;

	typedef void (*setHTTPResponseDefault)(void* implementation, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setHTTPResponseDefault, &exception);

	return exception;
}

WebFrameworkException setHTTPResponseIsValid(HTTPResponse implementation, bool isValid)
{
	WebFrameworkException exception = NULL;

	typedef void (*setHTTPResponseDefault)(void* implementation, bool isValid, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setHTTPResponseDefault, isValid, &exception);

	return exception;
}
