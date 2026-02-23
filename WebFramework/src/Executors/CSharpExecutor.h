#pragma once

#include <Executors/Executor.h>

#ifdef __WITH_DOTNET_EXECUTORS__

#include "Runtimes/DotNetRuntime.h"

namespace framework
{
	class CSharpExecutor : public Executor
	{
	private:
		void processMethod(runtime::Runtime& runtime, runtime::DotNetRuntime::DoMethodSignature method, interfaces::IHttpRequest& request, interfaces::IHttpResponse& response);

	private:
		void* implementation;

	public:
		CSharpExecutor(void* implementation);

		void init(const utility::JSONSettingsParser::ExecutorSettings& settings) override;

		void doGet(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doPost(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doHead(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doPut(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doDelete(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doPatch(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doOptions(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doTrace(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doConnect(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		utility::ExecutorType getType() const override;

		void destroy() override;

		~CSharpExecutor();
	};
}

#endif
