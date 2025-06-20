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

		void doPost(HTTPRequest& request, HTTPResponse& response) override;

		void doGet(HTTPRequest& request, HTTPResponse& response) override;

		void doHead(HTTPRequest& request, HTTPResponse& response) override;

		void doPut(HTTPRequest& request, HTTPResponse& response) override;

		void doDelete(HTTPRequest& request, HTTPResponse& response) override;

		void doPatch(HTTPRequest& request, HTTPResponse& response) override;

		void doOptions(HTTPRequest& request, HTTPResponse& response) override;

		void doTrace(HTTPRequest& request, HTTPResponse& response) override;

		void doConnect(HTTPRequest& request, HTTPResponse& response) override;

		utility::ExecutorType getType() const override;

		void destroy() override;

		~CXXExecutor() = default;
	};
}
