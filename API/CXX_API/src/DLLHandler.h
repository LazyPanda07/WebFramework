#pragma once

#include <filesystem>
#include <utility>

#ifdef __LINUX__
#include <dlfcn.h>

using HMODULE = void*;
#else
#include <Windows.h>
#endif

#define DEFINE_CLASS_MEMBER_FUNCTION(functionName, returnType, ...) using functionName = returnType (*)(void* implementation, __VA_ARGS__)
#define CALL_FUNCTION(functionName, ...) callFunction<functionName>(#functionName, __VA_ARGS__)
#define CALL_CLASS_MEMBER_FUNCTION(functionName, ...) callClassMemberFunction<functionName>(#functionName, implementation, __VA_ARGS__)

namespace framework
{
	class DLLHandler
	{
	private:
		HMODULE handle;

	public:
		DLLHandler(const std::filesystem::path& pathToDLL);

		template<typename T, typename... Args>
		auto callFunction(std::string_view functionName, Args&&... args);

		template<typename T, typename... Args>
		auto callClassMemberFunction(std::string_view functionName, void* implementation, Args&&... args);

		void free(void* implementation);

		~DLLHandler() = default;
	};
}

namespace framework
{
	inline DLLHandler::DLLHandler(const std::filesystem::path& pathToDLL)
	{
#ifdef __LINUX__
		handle = dlopen(pathToDLL.string().data(), RTLD_LAZY);
#else
		handle = LoadLibraryA(pathToDLL.string().data());
#endif
	}

	template<typename T, typename... Args>
	inline auto DLLHandler::callFunction(std::string_view functionName, Args&&... args)
	{
		T function;

#ifdef __LINUX__
		function = reinterpret_cast<T>(dlsym(handle, functionName.data()));
#else
		function = reinterpret_cast<T>(GetProcAddress(handle, functionName.data()));
#endif

		return function(std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	inline auto DLLHandler::callClassMemberFunction(std::string_view functionName, void* implementation, Args&&... args)
	{
		T function;

#ifdef __LINUX__
		function = reinterpret_cast<T>(dlsym(handle, functionName.data()));
#else
		function = reinterpret_cast<T>(GetProcAddress(handle, functionName.data()));
#endif

		return function(implementation, std::forward<Args>(args)...);
	}

	inline void DLLHandler::free(void* implementation)
	{
		using deleteWebFrameworkObject = void (*)(void* implementation);

		this->CALL_FUNCTION(deleteWebFrameworkObject, implementation);
	}
}
