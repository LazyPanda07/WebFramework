#pragma once

#include <string_view>
#include <any>
#include <optional>

#include <Executors/BaseExecutor.h>

#include "WebInterfaces/IHTTPRequest.h"
#include "Utility/Sources.h"

namespace framework::runtime
{
	class Runtime
	{
	public:
		Runtime() = default;

		virtual void finishInitialization() = 0;

		virtual bool loadExecutor(std::string_view name, const utility::LoadSource& source) = 0;

		virtual std::unique_ptr<BaseExecutor> createExecutor(std::string_view name) const = 0;

		virtual void* createExecutorSettings(const void* implementation) const = 0;

		virtual void* createHTTPRequest(framework::interfaces::IHTTPRequest* request) const = 0;

		virtual void* createHTTPResponse(framework::interfaces::IHTTPResponse* response) const = 0;

		virtual void initializeWebFramework(std::string_view libraryPath) = 0;

		virtual std::optional<std::string> loadSource(std::string_view pathToSource, utility::LoadSource& source) = 0;

		virtual ~Runtime() = default;
	};
}
