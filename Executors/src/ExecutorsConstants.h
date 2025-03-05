#pragma once

#include <string_view>

namespace json_settings
{
	inline constexpr std::string_view initParametersKey = "initParameters";
	inline constexpr std::string_view loadTypeKey = "loadType";
	inline constexpr std::string_view routeKey = "route";
}

namespace json_settings_values
{
	inline constexpr std::string_view initializationLoadTypeValue = "initialization";
	inline constexpr std::string_view dynamicLoadTypeValue = "dynamic";
}

#ifdef __LINUX__
#define EXECUTORS_API __attribute__((visibility("hidden")))
#else
#define EXECUTORS_API
#endif
