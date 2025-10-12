#pragma once

#include <string_view>

namespace framework::runtime
{
	class Runtime
	{
	public:
		Runtime() = default;

		virtual void initializeWebFramework(std::string_view libraryPath) = 0;

		virtual ~Runtime() = default;
	};
}
