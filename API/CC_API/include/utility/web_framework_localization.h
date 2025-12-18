#pragma once

#include "../dll_handler.h"

/**
 * @brief Get localized string from specific language
 * @param localizationModuleName Localization module name
 * @param key Localization string key
 * @param language Preferred language. If can't find string for that language then returns string in original language
 * @param result Localized string
 * @return Error if occurred
 */
web_framework_exception_t wf_get_localized_string(const char* localization_module_name, const char* key, const char* language, const char** result);
