#pragma once

#include "DynamicFunction.h"

namespace framework
{
	class CCDynamicFunction : public DynamicFunction
	{
	private:
		char* (*function)(const char**, size_t);
		void(*deleter)(char*);

	public:
		CCDynamicFunction(const void* data);

		std::string operator ()(const std::vector<std::string>& arguments) const override;

		~CCDynamicFunction() = default;
	};
}
