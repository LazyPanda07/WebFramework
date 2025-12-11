#include "utility/web_framework_localization.h"

web_framework_exception_t wf_get_localized_string(const char* localization_module_name, const char* key, const char* language, const char** result)
{
	web_framework_exception_t exception = NULL;

	typedef void* (*getWebFrameworkLocalizedString)(const char* localization_module_name, const char* key, const char* language, void** exception);

	*result = CALL_WEB_FRAMEWORK_FUNCTION(getWebFrameworkLocalizedString, localization_module_name, key, language, &exception);

	return exception;
}
