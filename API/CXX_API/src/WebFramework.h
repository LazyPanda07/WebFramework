#pragma once

#include <functional>

#include "DLLHandler.h"
#include "WebFrameworkException.h"

namespace framework_api
{
	class WebFramework
	{
	private:
		std::function<void()> onStartServer;
		std::shared_ptr<DLLHandler> handler;
		void* implementation;
		bool weak;

	public:
		WebFramework(std::shared_ptr<DLLHandler> handler, const std::string& config);

		WebFramework(std::shared_ptr<DLLHandler> handler, std::string_view serverConfiguration, std::string_view sourcesPath);

		WebFramework(const WebFramework& other);

		WebFramework(WebFramework&& other) noexcept = delete;

		WebFramework& operator = (const WebFramework& other);

		WebFramework& operator = (WebFramework&& other) noexcept = delete;

		void start(bool wait = false, const std::function<void()>& onStartServer = []() {});

		void stop(bool wait = true);

		~WebFramework();
	};
}

namespace framework_api
{
	WebFramework::WebFramework(std::shared_ptr<DLLHandler> handler, const std::string& config) :
		handler(handler),
		weak(false)
	{
		using createWebFrameworkFromPath = void* (*)(const char* configPath, void** exception);
		void* exception = nullptr;

		implementation = handler->CALL_FUNCTION(createWebFrameworkFromPath, config.data(), &exception);

		if (exception)
		{
			throw WebFrameworkException(handler, exception);
		}
	}

	WebFramework::WebFramework(std::shared_ptr<DLLHandler> handler, std::string_view serverConfiguration, std::string_view sourcesPath) :
		handler(handler),
		weak(false)
	{
		using createWebFrameworkFromString = void* (*)(const char* serverConfiguration, const char* sourcesPath, void** exception);
		void* exception = nullptr;

		implementation = handler->CALL_FUNCTION(createWebFrameworkFromString, serverConfiguration.data(), sourcesPath.data(), &exception);

		if (exception)
		{
			throw WebFrameworkException(handler, exception);
		}
	}

	WebFramework::WebFramework(const WebFramework& other)
	{
		(*this) = other;
	}

	WebFramework& WebFramework::operator = (const WebFramework& other)
	{
		handler = other.handler;
		implementation = other.implementation;
		weak = true;

		return *this;
	}

	void WebFramework::start(bool wait, const std::function<void()>& onStartServer)
	{
		DEFINE_CLASS_MEMBER_FUNCTION(startWebFrameworkServerCXX, void, bool wait, void* onStartServer, void** exception);
		void* exception = nullptr;

		this->onStartServer = onStartServer;

		handler->CALL_CLASS_MEMBER_FUNCTION(startWebFrameworkServerCXX, wait, static_cast<void*>(&this->onStartServer), &exception);

		if (exception)
		{
			throw WebFrameworkException(handler, exception);
		}
	}

	void WebFramework::stop(bool wait)
	{
		DEFINE_CLASS_MEMBER_FUNCTION(stopWebFrameworkServer, void, bool wait, void** exception);
		void* exception = nullptr;

		handler->CALL_CLASS_MEMBER_FUNCTION(stopWebFrameworkServer, wait, &exception);

		if (exception)
		{
			throw WebFrameworkException(handler, exception);
		}
	}

	WebFramework::~WebFramework()
	{
		if (!weak)
		{
			handler->free(implementation);
		}
	}
}
