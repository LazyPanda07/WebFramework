#pragma once

#include "../DLLHandler.h"

/**
 * @brief Get localized string from specific language
 * @param localizationModuleName Localization module name
 * @param key Localization string key
 * @param language Preferred language. If can't find string for that language then returns string in original language
 * @param result Localized string
 * @return Error if occurred
 */
WebFrameworkException getLocalizedString(const char* localizationModuleName, const char* key, const char* language, const char** result);
