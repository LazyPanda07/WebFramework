#pragma once

#include <Executors/Executor.h>

#include <concepts>

#include <Utility/Sources.h>

namespace framework
{
	class CCExecutor : public Executor
	{
	private:
		using InitExecutorSignature = void(*)(void*, const void*);
		using DoMethodSignature = void(*)(void*, void*, void*);
		using GetTypeSignature = int(*)(void*);
		using DestroySignature = void(*)(void*);
		using DeleteSignature = void(*)(void*);

	private:
		void* implementation;
		std::string executorName;
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

	private:
		template<typename T, typename... Args>
		void callBindedMethodFunction(const T& function, std::string_view functionName, Args&&... args) requires std::invocable<T, Args...>;

	public:
		CCExecutor(HMODULE module, void* implementation, std::string_view executorName);

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

		~CCExecutor();
	};
}
