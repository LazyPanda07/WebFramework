#pragma once

#include <string_view>

#include "WebInterfaces/IHTTPRequest.h"

namespace framework::runtime
{
	class Runtime
	{
	public:
		Runtime() = default;

		virtual void finishInitialization() = 0;

		virtual void* createExecutorSettings(const void* implementation) const = 0;

		virtual void* createHTTPRequest(framework::interfaces::IHTTPRequest* request) const = 0;

		virtual void* createHTTPResponse(framework::interfaces::IHTTPResponse* response) const = 0;

		virtual void initializeWebFramework(std::string_view libraryPath) = 0;

		virtual ~Runtime() = default;
	};
}
