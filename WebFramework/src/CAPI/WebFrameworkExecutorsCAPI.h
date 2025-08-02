#pragma once

#include "WebFrameworkUtilityCAPI.h"

typedef void* HTTPResponseObject;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPORT void setHTTPResponseBody(HTTPResponseObject response, const char* body, Exception* exception);
