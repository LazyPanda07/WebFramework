#pragma once

#include <Executors/Executor.h>

#include <Utility/Sources.h>
#include <WebInterfaces/IHttpRequest.h>
#include <WebInterfaces/IHttpResponse.h>

namespace framework
{
	class CXXExecutor : public Executor
	{
	private:
		using InitExecutorSignature = void(*)(void*, const void*);
		using DoMethodSignature = void(*)(void*, interfaces::IHttpRequest*, interfaces::IHttpResponse*);
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

		void doPost(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doGet(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doHead(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doPut(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doDelete(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doPatch(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doOptions(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doTrace(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		void doConnect(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response) override;

		utility::ExecutorType getType() const override;

		void destroy() override;

		~CXXExecutor();
	};
}
