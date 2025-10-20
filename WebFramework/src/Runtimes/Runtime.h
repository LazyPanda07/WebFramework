#pragma once

#include <string_view>
#include <any>

#include "WebInterfaces/IHTTPRequest.h"
#include "Utility/Sources.h"

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

		virtual std::any getClass(std::string_view className, const utility::LoadSource& source) const = 0;

		virtual ~Runtime() = default;
	};
}
