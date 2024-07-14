#pragma once

#include <filesystem>
#include <utility>
#include <vector>
#include <format>

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
	namespace utility
	{
		void initializeWebFramework(const std::filesystem::path& pathToDLL);

		class DLLHandler
		{
		private:
			static inline std::unique_ptr<DLLHandler> instance;

		private:
			HMODULE handle;

		private:
			DLLHandler(const std::filesystem::path& pathToDLL);

			~DLLHandler() = default;

		public:
			static DLLHandler& getInstance();

		public:
			template<typename T, typename... Args>
			auto callFunction(std::string_view functionName, Args&&... args);

			template<typename T, typename... Args>
			auto callClassMemberFunction(std::string_view functionName, void* implementation, Args&&... args);

			void free(void* implementation);

			friend void utility::initializeWebFramework(const std::filesystem::path& pathToDLL);
			friend struct std::default_delete<DLLHandler>;
		};
	}
}

namespace framework
{
	namespace utility
	{
		inline void initializeWebFramework(const std::filesystem::path& pathToDLL)
		{
			if (DLLHandler::instance)
			{
				return;
			}

			auto makePathToDLL = [](const std::filesystem::path& pathToSource) -> std::filesystem::path
				{
#ifndef __LINUX__
					return std::format("{}.dll", pathToSource.string());

#else
					std::filesystem::path parent = pathToSource.parent_path();
					std::filesystem::path fileName = pathToSource.filename();

					return std::format("{}/lib{}.so", parent.string(), fileName.string());
#endif
				};

			std::filesystem::path realPath = makePathToDLL(std::filesystem::absolute(pathToDLL));

			if (!std::filesystem::exists(realPath))
			{
				throw std::runtime_error(std::format("Path {} doesn't exist", realPath.string()));
			}

			DLLHandler::instance = std::unique_ptr<DLLHandler>(new DLLHandler(realPath));
		}


		inline DLLHandler::DLLHandler(const std::filesystem::path& pathToDLL)
		{
#ifdef __LINUX__
			handle = dlopen(pathToDLL.string().data(), RTLD_LAZY);
#else
			handle = LoadLibraryA(pathToDLL.string().data());
#endif

			if (!handle)
			{
#ifdef __LINUX__
				throw std::runtime_error(std::format("Can't load {} or its dependencies", pathToDLL.string()));
#else
				throw std::runtime_error(std::format("GetLastError(): {}", GetLastError()));
#endif
			}
		}

		inline DLLHandler& DLLHandler::getInstance()
		{
			if (!DLLHandler::instance)
			{
				throw std::runtime_error("WebFramework must be initialized with initializeWebFramework function");
			}

			return *DLLHandler::instance;
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
}
