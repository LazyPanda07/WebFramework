#pragma once

#include "DLLHandler.h"

typedef void* HTTPResponse;

WebFrameworkException setBody(HTTPResponse implementation, const char* body);

inline WebFrameworkException setBody(HTTPResponse implementation, const char* body)
{
	WebFrameworkException exception = NULL;

	typedef void (*setHTTPResponseBody)(void* implementation, const char* body, void** exception);

	CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(setHTTPResponseBody, body, &exception);

	return exception;
}
