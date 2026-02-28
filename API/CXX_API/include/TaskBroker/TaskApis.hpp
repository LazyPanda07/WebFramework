#pragma once

#include <string_view>

namespace framework::task_broker
{
	struct CXXApi
	{
		static inline constexpr std::string_view taskBrokerApi = "cxx";
	};

	struct CCApi
	{
		static inline constexpr std::string_view taskBrokerApi = "cc";
	};

	struct PythonApi
	{
		static inline constexpr std::string_view taskBrokerApi = "python";
	};

	struct CSharpApi
	{
		static inline constexpr std::string_view taskBrokerApi = "csharp";
	};
}
