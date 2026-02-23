#pragma once

#include <string>
#include <vector>
#include <functional>

namespace framework::utility
{
	enum class ExecutorApiType
	{
		cc,
		cxx,
		python,
		csharp,
		count
	};

	ExecutorApiType getExecutorApiType(std::string_view name);

	std::function<std::string(const std::vector<std::string>&)> createCxxDynamicFunction(const char* (*function)(const char** arguments, size_t argumentsNumber), void(*deleter)(char* result));
}
