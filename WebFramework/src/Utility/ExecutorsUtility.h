#pragma once

#include <string_view>

namespace framework::utility
{
	enum class ExecutorAPIType
	{
		cc,
		cxx,
		python,
		count
	};

	ExecutorAPIType getExecutorAPIType(std::string_view name);
}
