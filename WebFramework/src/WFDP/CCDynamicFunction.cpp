#include "WFDP/CCDynamicFunction.h"

namespace framework
{
	CCDynamicFunction::CCDynamicFunction(const void* data) :
		function(nullptr),
		deleter(nullptr)
	{
		const void* const* functions = reinterpret_cast<const void* const*>(data);

		function = reinterpret_cast<char* (*)(const char**, size_t)>(functions[0]);
		deleter = reinterpret_cast<void (*)(char*)>(functions[1]);
	}

	std::string CCDynamicFunction::operator()(const std::vector<std::string>& arguments) const
	{
		std::vector<const char*> pointers;
		std::string result;

		pointers.reserve(arguments.size());

		for (const std::string& argument : arguments)
		{
			pointers.emplace_back(argument.data());
		}

		char* temp = function(pointers.data(), pointers.size());
		result = temp;

		deleter(temp);

		return result;
	}
}
