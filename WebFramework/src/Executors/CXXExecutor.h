#pragma once

#include <Executors/BaseExecutor.h>

#include <Utility/Sources.h>
#include <WebNetwork/Interfaces/IHTTPRequest.h>
#include <WebNetwork/Interfaces/IHTTPResponse.h>

namespace framework
{
	class CXXExecutor : public BaseExecutor
	{
	public:
		using DoMethodSignature = void(*)(void*, interfaces::IHTTPRequest*, interfaces::IHTTPResponse*);
		using GetTypeSignature = utility::ExecutorType(*)(void*);
		using DestroySignature = void(*)(void*);

	private:
		void* implementation;
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

	public:
		CXXExecutor(HMODULE module, void* implementation);

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

		~CXXExecutor() = default;
	};
}
