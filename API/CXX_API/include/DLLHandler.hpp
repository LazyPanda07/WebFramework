#pragma once

#include <filesystem>
#include <utility>
#include <vector>
#include <format>
#include <fstream>

#ifdef __LINUX__
#include <dlfcn.h>

using HMODULE = void*;
#else
#include <Windows.h>
#endif

#define DEFINE_CLASS_MEMBER_FUNCTION(functionName, returnType, ...) using functionName = returnType (*)(void* implementation, __VA_ARGS__)
#define CALL_WEB_FRAMEWORK_FUNCTION(functionName, ...) callFunction<functionName>(#functionName, __VA_ARGS__)
#define CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(functionName, ...) callClassMemberFunction<functionName>(#functionName, implementation, __VA_ARGS__)

namespace framework::utility
{
	/**
	 * @brief Load WebFramework shared library
	 * @param pathToDLL Path to shared library without prefixes(lib for Linux) and file extensions(.dll, .so)
	 */
	void initializeWebFramework(const std::filesystem::path& pathToDLL = "");

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

		void deleteString(void* implementation);

		void deleteConfig(void* implementation);

		void deleteWebFramework(void* implementation);

		void deleteException(void* implementation);

		void deleteJsonObject(void* implementation);

		void deleteJsonBuilder(void* implementation);

		void deleteJsonParser(void* implementation);

		/**
		 * @brief Get data from string and delete it
		 * @param implementation
		 * @return
		 */
		std::string getString(void* implementation);

		friend void utility::initializeWebFramework(const std::filesystem::path& pathToDLL);
		friend struct std::default_delete<DLLHandler>;
	};
}

namespace framework::utility
{
	inline void initializeWebFramework(const std::filesystem::path& pathToDLL)
	{
		if (DLLHandler::instance)
		{
			return;
		}

#ifdef __ANDROID__
		DLLHandler::instance = std::unique_ptr<DLLHandler>(new DLLHandler("libWebFramework.so"));

		return;
#endif

		auto makePathToDLL = [](const std::filesystem::path& pathToSource) -> std::filesystem::path
			{
				std::filesystem::path absolutePath = pathToSource == "WebFramework" ?
					"WebFramework" :
					std::filesystem::absolute(pathToSource);

				std::string fileName = absolutePath.filename().string();
				std::string extension;
				std::string prefix;

				if (!absolutePath.has_extension())
				{
#ifdef __LINUX__
					extension = ".so";
#else
					extension = ".dll";
#endif
				}

				absolutePath.remove_filename();

#ifdef __LINUX__
				if (fileName.find("lib") == std::string::npos)
				{
					prefix = "lib";
				}
#endif

				return std::format("{}{}{}{}", absolutePath.string(), prefix, fileName, extension);
			};

		std::filesystem::path realPath = makePathToDLL(pathToDLL.empty() ? "WebFramework" : std::filesystem::absolute(pathToDLL));

		if (realPath.is_absolute() && !std::filesystem::exists(realPath))
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

	inline void DLLHandler::deleteString(void* implementation)
	{
		using deleteWebFrameworkString = void (*)(void* implementation);

		this->CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkString, implementation);
	}

	inline void DLLHandler::deleteConfig(void* implementation)
	{
		using deleteWebFrameworkConfig = void (*)(void* implementation);

		this->CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkConfig, implementation);
	}

	inline void DLLHandler::deleteWebFramework(void* implementation)
	{
		using deleteWebFramework = void (*)(void* implementation);

		this->CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFramework, implementation);
	}

	inline void DLLHandler::deleteException(void* implementation)
	{
		using deleteWebFrameworkException = void (*)(void* implementation);

		this->CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkException, implementation);
	}

	inline void DLLHandler::deleteJsonObject(void* implementation)
	{
		using deleteWebFrameworkJsonObject = void (*)(void* implementation);

		this->CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkJsonObject, implementation);
	}

	inline void DLLHandler::deleteJsonBuilder(void* implementation)
	{
		using deleteWebFrameworkJsonBuilder = void (*)(void* implementation);

		this->CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkJsonBuilder, implementation);
	}

	inline void DLLHandler::deleteJsonParser(void* implementation)
	{
		using deleteWebFrameworkJsonParser = void (*)(void* implementation);

		this->CALL_WEB_FRAMEWORK_FUNCTION(deleteWebFrameworkJsonParser, implementation);
	}

	inline std::string DLLHandler::getString(void* implementation)
	{
		using getDataFromString = const char* (*)(void* implementation);

		std::string result(CALL_WEB_FRAMEWORK_FUNCTION(getDataFromString, implementation));

		this->deleteString(implementation);

		return result;
	}
}
