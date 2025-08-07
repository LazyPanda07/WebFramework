#pragma once

#include "WebFrameworkUtilityCAPI.h"

typedef void* HTTPResponseObject;
typedef void* HTTPRequestObject;

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

EXPORT const char* getHTTPRawParameters(HTTPRequestObject request, Exception* exception);

EXPORT const char* getHTTPMethod(HTTPRequestObject request, Exception* exception);

EXPORT void* getHTTPVersion(HTTPRequestObject request, Exception* exception);

EXPORT const char* getHTTPBody(HTTPRequestObject request, Exception* exception);

EXPORT void setHTTPAttribute(HTTPRequestObject request, const char* name, const char* value, Exception* exception);

EXPORT void* getHTTPAttribute(HTTPRequestObject request, const char* name, Exception* exception);

EXPORT void deleteHTTPSession(HTTPRequestObject request, Exception* exception);

EXPORT void removeHTTPAttribute(HTTPRequestObject request, const char* name, Exception* exception);

EXPORT JSONParser getHTTPRequestJSON(HTTPRequestObject request, Exception* exception);

EXPORT const char* getHTTPRawRequest(HTTPRequestObject request, Exception* exception);

EXPORT void* getClientIpV4(HTTPRequestObject request, Exception* exception);

EXPORT void* getServerIpV4(HTTPRequestObject request, Exception* exception);

EXPORT uint16_t getClientPort(HTTPRequestObject request, Exception* exception);

EXPORT uint16_t getServerPort(HTTPRequestObject request, Exception* exception);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
