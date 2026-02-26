#pragma once

#include <string_view>
#include <any>
#include <optional>

#include <Executors/Executor.h>

#include "WebInterfaces/IHttpRequest.h"
#include "Utility/Sources.h"

namespace framework::runtime
{
	class Runtime
	{
	public:
		Runtime() = default;

		virtual void finishInitialization() = 0;

		virtual bool loadExecutor(std::string_view name, std::string_view route, const utility::LoadSource& source) = 0;

		virtual std::unique_ptr<Executor> createExecutor(std::string_view name) const = 0;

		virtual void* createExecutorSettings(const void* implementation) const = 0;

		virtual void* createHTTPRequest(framework::interfaces::IHttpRequest* request) const = 0;

		virtual void* createHTTPResponse(framework::interfaces::IHttpResponse* response) const = 0;

		virtual void initializeWebFramework(const utility::LoadSource& source, std::string_view libraryPath) = 0;

		virtual std::optional<std::string> loadSource(std::string_view pathToSource, utility::LoadSource& source) = 0;

		virtual ~Runtime() = default;
	};
}
