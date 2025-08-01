#pragma once

#include <Executors/BaseExecutor.h>

#include <concepts>

#include <Utility/Sources.h>

namespace framework
{
	class CCExecutor : public BaseExecutor
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
		void callBindedFunction(const T& function, std::string_view functionName, Args&&... args) requires std::invocable<T, Args...>;

	public:
		CCExecutor(HMODULE module, void* implementation, std::string_view executorName);

		void init(const utility::JSONSettingsParser::ExecutorSettings& settings) override;

		void doPost(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doGet(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doHead(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doPut(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doDelete(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doPatch(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doOptions(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doTrace(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		void doConnect(HTTPRequestExecutors& request, HTTPResponseExecutors& response) override;

		utility::ExecutorType getType() const override;

		void destroy() override;

		~CCExecutor();
	};
}
