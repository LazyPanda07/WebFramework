#pragma once

#include <functional>
#include <filesystem>

#include "DLLHandler.hpp"
#include "Exceptions/WebFrameworkException.hpp"
#include "Config.hpp"

namespace framework
{
	/**
	 * @brief Web server
	 */
	class WebFramework
	{
	private:
		std::function<void()> onStartServer;
		void* implementation;
		bool weak;

	public:
		/**
		 * @brief Get WebFramework version
		 * @return String representation of version in format {major}.{minor}.{patch}
		 */
		static std::string_view getWebFrameworkVersion();

	public:
		/**
		 * @brief 
		 * @param configPath Path to *.json config
		 */
		WebFramework(std::string_view configPath);

		/**
		 * @brief 
		 * @param serverConfiguration *.json config file content 
		 * @param applicationDirectory Working directory
		 */
		WebFramework(std::string_view serverConfiguration, std::string_view applicationDirectory);

		/**
		 * @brief 
		 * @param config Config instance
		 */
		WebFramework(const utility::Config& config);

		WebFramework(const WebFramework& other);

		WebFramework(WebFramework&& other) noexcept = delete;

		WebFramework& operator = (const WebFramework& other);

		WebFramework& operator = (WebFramework&& other) noexcept = delete;

		/**
		 * @brief Start server
		 * @param wait Wait until server stop
		 * @param onStartServer On start server callback
		 */
		void start(bool wait = false, const std::function<void()>& onStartServer = []() {});

		/**
		 * @brief Stop server
		 * @param wait Wait until server stop
		 */
		void stop(bool wait = true);

		/**
		 * @brief Is server running
		 * @return 
		 */
		bool isServerRunning() const;

		~WebFramework();
	};
}

namespace framework
{
	inline std::string_view WebFramework::getWebFrameworkVersion()
	{
		return utility::DLLHandler::getInstance().callFunction<const char* (*)()>("getWebFrameworkVersion");
	}

	inline WebFramework::WebFramework(std::string_view configPath) :
		weak(false)
	{
		using createWebFrameworkFromPath = void* (*)(const char* configPath, void** exception);

		if (!std::filesystem::exists(configPath))
		{
			throw std::runtime_error(std::format("Path {} doesn't exist", configPath));
		}

		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createWebFrameworkFromPath, configPath.data(), &exception);

		if (exception) //-V547
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline WebFramework::WebFramework(std::string_view serverConfiguration, std::string_view applicationDirectory) :
		weak(false)
	{
		using createWebFrameworkFromString = void* (*)(const char* serverConfiguration, const char* applicationDirectory, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createWebFrameworkFromString, serverConfiguration.data(), applicationDirectory.data(), &exception);

		if (exception) //-V547
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline WebFramework::WebFramework(const utility::Config& config) :
		weak(false)
	{
		using createWebFrameworkFromConfig = void* (*)(void* config, void** exception);
		void* exception = nullptr;

		implementation = utility::DLLHandler::getInstance().CALL_WEB_FRAMEWORK_FUNCTION(createWebFrameworkFromConfig, config.getImplementation(), &exception);

		if (exception) //-V547
		{
			throw exceptions::WebFrameworkException(exception);
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

		utility::DLLHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(startWebFrameworkServerCXX, wait, static_cast<void*>(&this->onStartServer), &exception);

		if (exception) //-V547
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline void WebFramework::stop(bool wait)
	{
		DEFINE_CLASS_MEMBER_FUNCTION(stopWebFrameworkServer, void, bool wait, void** exception);
		void* exception = nullptr;

		utility::DLLHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(stopWebFrameworkServer, wait, &exception);

		if (exception) //-V547
		{
			throw exceptions::WebFrameworkException(exception);
		}
	}

	inline bool WebFramework::isServerRunning() const
	{
		DEFINE_CLASS_MEMBER_FUNCTION(isServerRunning, bool, void** exception);
		void* exception = nullptr;

		bool result = utility::DLLHandler::getInstance().CALL_CLASS_MEMBER_WEB_FRAMEWORK_FUNCTION(isServerRunning, &exception);

		if (exception)
		{
			throw exceptions::WebFrameworkException(exception);
		}

		return result;
	}

	inline WebFramework::~WebFramework()
	{
		if (!weak)
		{
			utility::DLLHandler::getInstance().deleteWebFramework(implementation);
		}
	}
}
