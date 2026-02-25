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
		static void call(void* dynamicFunction, const std::span<std::string_view>& arguments, void* data, void(*callback)(const char* result, size_t size, void* data));

		static void deleter(void* implementation);

	public:
		DynamicFunction() = default;

		virtual std::string operator ()(const std::span<std::string_view>& arguments) = 0;

		~DynamicFunction() = default;
	};
}

namespace framework
{
	inline void DynamicFunction::call(void* dynamicFunction, const std::span<std::string_view>& arguments, void* data, void(*callback)(const char* result, size_t size, void* data))
	{
		std::string result = (*static_cast<DynamicFunction*>(dynamicFunction))(arguments);

		callback(result.data(), result.size(), data);
	}

	inline void DynamicFunction::deleter(void* implementation)
	{
		delete static_cast<DynamicFunction*>(implementation);
	}
}
