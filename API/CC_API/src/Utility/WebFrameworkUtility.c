#include "Utility/WebFrameworkUtility.h"

WebFrameworkException generateUUID(WebFrameworkString* result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*generateWebFrameworkUUID)(void** exception);

	*result = CALL_WEB_FRAMEWORK_FUNCTION(generateWebFrameworkUUID, &exception);

	return exception;
}
