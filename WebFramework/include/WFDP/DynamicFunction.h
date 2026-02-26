#pragma once

#include <string>
#include <vector>

namespace framework
{
	class DynamicFunction
	{
	public:
		DynamicFunction() = default;

		virtual std::string operator ()(const std::vector<std::string>& arguments) const = 0;

		virtual ~DynamicFunction() = default;
	};
}
