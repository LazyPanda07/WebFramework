#pragma once

#include <string_view>

namespace framework::task_broker
{
	struct CXXApi
	{
		static inline constexpr std::string_view messageBrokerApi = "cxx";
	};

	struct CCApi
	{
		static inline constexpr std::string_view messageBrokerApi = "cc";
	};

	struct PythonApi
	{
		static inline constexpr std::string_view messageBrokerApi = "python";
	};

	struct CSharpApi
	{
		static inline constexpr std::string_view messageBrokerApi = "csharp";
	};
}
