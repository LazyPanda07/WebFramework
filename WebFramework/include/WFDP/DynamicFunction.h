#pragma once

#include <string>

#include <JsonObject.h>

namespace framework
{
	class DynamicFunction
	{
	public:
		DynamicFunction() = default;

		virtual std::string operator ()(const json::JsonObject& arguments) const = 0;

		virtual ~DynamicFunction() = default;
	};
}
