#pragma once

#include <string_view>

namespace json_settings
{
	inline constexpr std::string_view initParametersKey = "initParameters";
	inline constexpr std::string_view loadTypeKey = "loadType";
	inline constexpr std::string_view routeKey = "route";
	inline constexpr std::string_view apiTypeKey = "api";

	inline constexpr std::string_view cxxExecutorKey = "cxx";
	inline constexpr std::string_view ccExecutorKey = "cc";
	inline constexpr std::string_view pythonExecutorKey = "python";
	inline constexpr std::string_view csharpExecutorKey = "csharp";
}

namespace json_settings_values
{
	inline constexpr std::string_view initializationLoadTypeValue = "initialization";
	inline constexpr std::string_view dynamicLoadTypeValue = "dynamic";
}

namespace framework::utility
{
	enum class ExecutorType
	{
		stateful,
		stateless,
		heavyOperationStateful,
		heavyOperationStateless
	};
}

#ifdef __LINUX__
#define WEB_FRAMEWORK_EXECUTORS_API __attribute__((visibility("hidden")))
#define WEB_FRAMEWORK_EXECUTORS_EXPORT_API __attribute__((visibility("hidden")))
#else
#define WEB_FRAMEWORK_EXECUTORS_API
#define WEB_FRAMEWORK_EXECUTORS_EXPORT_API
#endif
