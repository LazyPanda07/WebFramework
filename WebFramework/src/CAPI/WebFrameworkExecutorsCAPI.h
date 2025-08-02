#pragma once

#include "WebFrameworkUtilityCAPI.h"

typedef void* HTTPResponseObject;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT void setHTTPResponseBody(HTTPResponseObject response, const char* body, Exception* exception);

EXPORT void setHTTPResponseJSONBody(HTTPResponseObject response, JSONBuilder builder, Exception* exception);

EXPORT void setHTTPVersion(HTTPResponseObject response, const char* version, Exception* exception);

EXPORT void setHTTPResponseCode(HTTPResponseObject response, int64_t responseCode, Exception* exception);

EXPORT void addHTTPResponseHeader(HTTPResponseObject response, const char* name, const char* value, Exception* exception);

EXPORT void appendHTTPResponseBody(HTTPResponseObject response, const char* body, Exception* exception);

EXPORT void addHTTPResponseCookie(HTTPResponseObject response, const char* name, const char* value, Exception* exception);

EXPORT void setHTTPResponseDefault(HTTPResponseObject response, Exception* exception);

EXPORT void setHTTPResponseIsValid(HTTPResponseObject response, bool isValid, Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
