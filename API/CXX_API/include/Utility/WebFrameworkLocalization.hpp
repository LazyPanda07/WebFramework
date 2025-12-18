#pragma once

#include "../DLLHandler.hpp"
#include "../Exceptions/WebFrameworkException.hpp"

namespace framework::utility
{
	/**
	 * @brief Get localized string from specific language
	 * @param localizationModuleName Localization module name
	 * @param key Localization string key
	 * @param language Preferred language. If can't find string for that language then returns string in original language
	 * @return
	 */
	std::string_view getLocalizedString(std::string_view localizationModuleName, std::string_view key, std::string_view language = "");
}

namespace framework::utility
{
	inline std::string_view getLocalizedString(std::string_view localizationModuleName, std::string_view key, std::string_view language)
	{
		using getWebFrameworkLocalizedString = const char* (*)(const char* localizationModuleName, const char* key, const char* language, void** exception);
		void* exception = nullptr;

		const char* result = DllHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(getWebFrameworkLocalizedString, localizationModuleName.data(), key.data(), language.data(), &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return result;
	}
}
