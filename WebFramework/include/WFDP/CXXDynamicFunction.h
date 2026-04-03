#pragma once

#include "DynamicFunction.h"

#include <functional>
#include <variant>
#include <span>

namespace framework
{
	class CXXDynamicFunction : public DynamicFunction
	{
	public:
		struct DynamicFunctionController
		{
			void* dynamicFunction;
			void (*callFunction)(void* dynamicFunction, const std::span<std::string_view>& arguments, void* data, void(*callback)(const char* result, size_t size, void* data));
			void (*deleter)(void* implementation);

			DynamicFunctionController();
		};

	private:
		void* implementation;
		void (*callFunction)(void* dynamicFunction, const std::span<std::string_view>& arguments, void* data, void(*callback)(const char* result, size_t size, void* data));
		void (*deleter)(void* implementation);

	public:
		CXXDynamicFunction(const void* data);

		std::string operator ()(const std::vector<std::string>& arguments) const override;

		~CXXDynamicFunction();
	};
}
