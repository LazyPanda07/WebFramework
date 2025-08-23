#pragma once

#include "../DLLHandler.h"

WebFrameworkException getLocalizedString(const char* localizationModuleName, const char* key, const char* language, const char** result);
