#pragma once

#include <string>

#ifdef __LINUX__
#define LOCALIZATION_API __attribute__((visibility("default")))
#else
#define LOCALIZATION_API __declspec(dllexport)

#pragma warning(disable: 4251)
#endif

namespace localization
{
	inline constexpr std::string_view localizationModulesFile = "localization_modules.json";

	namespace settings
	{
		inline const std::string defaultModuleSetting = "defaultModule";
		inline const std::string modulesSetting = "modules";
	}
}
