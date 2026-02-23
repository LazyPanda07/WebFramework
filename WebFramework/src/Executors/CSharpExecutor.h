#pragma once

#include <Executors/Executor.h>

#ifdef __WITH_DOTNET_EXECUTORS__

#include "Runtimes/DotNetRuntime.h"

namespace framework
{
	class CSharpExecutor : public Executor
	{
	private:
		void processMethod(runtime::Runtime& runtime, runtime::DotNetRuntime::DoMethodSignature method, interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response);

	private:
		void* implementation;

	public:
		CSharpExecutor(void* implementation);

		void init(const utility::JSONSettingsParser::ExecutorSettings& settings) override;

		void doGet(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doPost(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doHead(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doPut(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doDelete(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doPatch(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doOptions(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doTrace(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doConnect(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		utility::ExecutorType getType() const override;

		void destroy() override;

		~CSharpExecutor();
	};
}

#endif
