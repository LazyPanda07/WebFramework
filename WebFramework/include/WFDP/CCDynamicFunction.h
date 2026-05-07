#pragma once

#include "DynamicFunction.h"

namespace framework
{
	class CCDynamicFunction : public DynamicFunction
	{
	private:
		typedef struct json_object
		{
			void* implementation;
			bool weak;
		} json_object_t;

	private:
		char* (*function)(json_object_t);
		void(*deleter)(char*);

	public:
		CCDynamicFunction(const void* data);

		std::string operator ()(const json::JsonObject& arguments) const override;

		~CCDynamicFunction() = default;
	};
}
