#include "WebFrameworkLocalizationCAPI.h"

#include <MultiLocalizationManager.h>
#include <Log.h>

#define LOG_EXCEPTION() if (Log::isValid()) { Log::error("Exception: {} in {} function", "C_API", e.what(), __func__); }
#define CREATE_EXCEPTION() *exception = new std::runtime_error(e.what())
#define LOG_AND_CREATE_EXCEPTION() LOG_EXCEPTION(); CREATE_EXCEPTION()
#define UNEXPECTED_EXCEPTION() if (Log::isValid()) { Log::error("Somethind went wrong", "C_API"); } *exception = new std::runtime_error(std::format("Something went wrong in file: {} on line: {}", __FILE__, __LINE__));

const char* getWebFrameworkLocalizedString(const char* localizationModuleName, const char* key, const char* language, Exception* exception)
{
	try
	{
		return localization::MultiLocalizationManager::getManager().getLocalizedString(localizationModuleName, key, language).data();
	}
	catch (const std::exception& e)
	{
		LOG_AND_CREATE_EXCEPTION();
	}
	catch (...)
	{
		UNEXPECTED_EXCEPTION();
	}

	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
