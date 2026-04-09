#include "WFDP/CCDynamicFunction.h"

#include <Log.h>

namespace framework
{
	CCDynamicFunction::CCDynamicFunction(const void* data) :
		function(nullptr),
		deleter(nullptr)
	{
		void** functions = reinterpret_cast<void**>(const_cast<void*>(data));

		function = reinterpret_cast<char* (*)(json_object_t)>(functions[0]);
		deleter = reinterpret_cast<void (*)(char*)>(functions[1]);
	}

	std::string CCDynamicFunction::operator()(const json::JsonObject& arguments) const
	{
		json_object_t object =
		{
			.implementation = &const_cast<json::JsonObject&>(arguments),
			.weak = true
		};

		if (Log::isValid())
		{
			Log::info("Input arguments for cc function: {}", "LogTemp", static_cast<std::string>(arguments));
		}

		char* temp = function(object);
		
		std::string result = temp;

		deleter(temp);

		return result;
	}
}
