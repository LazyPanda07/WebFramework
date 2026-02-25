#pragma once

#include "DynamicFunction.h"

#include <functional>
#include <any>
#include <span>

namespace framework
{
	class CXXDynamicFunction : public DynamicFunction
	{
	private:
		std::function<std::string(const std::vector<std::string>&)> functor;
		void* implementation;
		void (*callFunction)(void* dynamicFunction, const std::span<std::string_view>& arguments, void* data, void(*callback)(const char* result, size_t size, void* data));
		void (*deleter)(void* implementation);

	public:
		CXXDynamicFunction(const std::any& data);

		std::string operator ()(const std::vector<std::string>& arguments) const override;

		~CXXDynamicFunction();
	};
}
