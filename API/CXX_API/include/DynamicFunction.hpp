#pragma once

#include <span>
#include <string_view>

namespace framework
{
	class DynamicFunction
	{
	public:
		static constexpr std::string_view dynamicFunctionImplementationName = "cxx";

	public:
		DynamicFunction() = default;

		virtual std::string_view operator ()(const std::span<std::string_view>& arguments) = 0;

		~DynamicFunction() = default;
	};
}
