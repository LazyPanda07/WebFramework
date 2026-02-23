#pragma once

#include <Executors/Executor.h>

#include <Utility/Sources.h>
#include <WebInterfaces/IHTTPRequest.h>
#include <WebInterfaces/IHTTPResponse.h>

namespace framework
{
	class CXXExecutor : public Executor
	{
	private:
		using InitExecutorSignature = void(*)(void*, const void*);
		using DoMethodSignature = void(*)(void*, interfaces::IHTTPRequest*, interfaces::IHTTPResponse*);
		using GetTypeSignature = utility::ExecutorType(*)(void*);
		using DestroySignature = void(*)(void*);
		using DeleteSignature = void(*)(void*);

	private:
		void* implementation;
		InitExecutorSignature initFunction;
		DoMethodSignature doPostFunction;
		DoMethodSignature doGetFunction;
		DoMethodSignature doHeadFunction;
		DoMethodSignature doPutFunction;
		DoMethodSignature doDeleteFunction;
		DoMethodSignature doPatchFunction;
		DoMethodSignature doOptionsFunction;
		DoMethodSignature doTraceFunction;
		DoMethodSignature doConnectFunction;
		GetTypeSignature getTypeFunction;
		DestroySignature destroyFunction;
		DeleteSignature deleteFunction;

	public:
		CXXExecutor(HMODULE module, void* implementation);

		void init(const utility::JSONSettingsParser::ExecutorSettings& settings) override;

		void doPost(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doGet(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doHead(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doPut(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doDelete(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doPatch(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doOptions(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doTrace(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		void doConnect(interfaces::IHTTPRequest& request, interfaces::IHTTPResponse& response) override;

		utility::ExecutorType getType() const override;

		void destroy() override;

		~CXXExecutor();
	};
}
