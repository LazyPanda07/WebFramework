#include "Utility/WebFrameworkLocalization.h"

WebFrameworkException getLocalizedString(const char* localizationModuleName, const char* key, const char* language, const char** result)
{
	WebFrameworkException exception = NULL;

	typedef void* (*getWebFrameworkLocalizedString)(const char* localizationModuleName, const char* key, const char* language, void** exception);

	*result = CALL_WEB_FRAMEWORK_FUNCTION(getWebFrameworkLocalizedString, localizationModuleName, key, language, &exception);

	return exception;
}
