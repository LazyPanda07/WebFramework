#pragma once

#include <functional>
#include <filesystem>

#include "DLLHandler.h"
#include "WebFrameworkException.h"
#include "Config.h"

namespace framework
{
	void initializeWebFramework(const std::filesystem::path& pathToDLL);

	class WebFramework
	{
	private:
		std::function<void()> onStartServer;
		void* implementation;
		bool weak;

	public:
		WebFramework(const std::string& config);

		WebFramework(std::string_view serverConfiguration, std::string_view sourcesPath);

		WebFramework(const utility::Config& config);

		WebFramework(const WebFramework& other);

		WebFramework(WebFramework&& other) noexcept = delete;

		WebFramework& operator = (const WebFramework& other);

		WebFramework& operator = (WebFramework&& other) noexcept = delete;

		void start(bool wait = false, const std::function<void()>& onStartServer = []() {});

		void stop(bool wait = true);

		~WebFramework();
	};
}

namespace framework
{
	inline void initializeWebFramework(const std::filesystem::path& pathToDLL)
	{
		if (DLLHandler::instance)
		{
			throw std::runtime_error("WebFramework already initialized");
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

	inline WebFramework::WebFramework(const std::string& config) :
		weak(false)
	{
		using createWebFrameworkFromPath = void* (*)(const char* configPath, void** exception);

		if (!std::filesystem::exists(config))
		{
			throw std::runtime_error(std::format("Path {} doesn't exist", config));
		}

		void* exception = nullptr;
		
		implementation = DLLHandler::getInstance().CALL_FUNCTION(createWebFrameworkFromPath, config.data(), &exception);

		if (exception)
		{
			throw WebFrameworkException(exception);
		}
	}

	inline WebFramework::WebFramework(std::string_view serverConfiguration, std::string_view sourcesPath) :
		weak(false)
	{
		using createWebFrameworkFromString = void* (*)(const char* serverConfiguration, const char* sourcesPath, void** exception);
		void* exception = nullptr;
		
		implementation = DLLHandler::getInstance().CALL_FUNCTION(createWebFrameworkFromString, serverConfiguration.data(), sourcesPath.data(), &exception);

		if (exception)
		{
			throw WebFrameworkException(exception);
		}
	}

	inline WebFramework::WebFramework(const utility::Config& config) :
		weak(false)
	{
		using createWebFrameworkFromConfig = void* (*)(void* config, void** exception);
		void* exception = nullptr;
		
		implementation = DLLHandler::getInstance().CALL_FUNCTION(createWebFrameworkFromConfig, config.getImplementation(), &exception);

		if (exception)
		{
			throw WebFrameworkException(exception);
		}
	}

	inline WebFramework::WebFramework(const WebFramework& other)
	{
		(*this) = other;
	}

	inline WebFramework& WebFramework::operator = (const WebFramework& other)
	{
		implementation = other.implementation;
		weak = true;

		return *this;
	}

	inline void WebFramework::start(bool wait, const std::function<void()>& onStartServer)
	{
		DEFINE_CLASS_MEMBER_FUNCTION(startWebFrameworkServerCXX, void, bool wait, void* onStartServer, void** exception);
		void* exception = nullptr;

		this->onStartServer = onStartServer;

		DLLHandler::getInstance().CALL_CLASS_MEMBER_FUNCTION(startWebFrameworkServerCXX, wait, static_cast<void*>(&this->onStartServer), &exception);

		if (exception)
		{
			throw WebFrameworkException(exception);
		}
	}

	inline void WebFramework::stop(bool wait)
	{
		DEFINE_CLASS_MEMBER_FUNCTION(stopWebFrameworkServer, void, bool wait, void** exception);
		void* exception = nullptr;

		DLLHandler::getInstance().CALL_CLASS_MEMBER_FUNCTION(stopWebFrameworkServer, wait, &exception);

		if (exception)
		{
			throw WebFrameworkException(exception);
		}
	}

	inline WebFramework::~WebFramework()
	{
		if (!weak)
		{
			DLLHandler::getInstance().free(implementation);
		}
	}
}
