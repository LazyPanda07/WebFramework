#include "Executors/CXXExecutor.h"

#include <format>

namespace framework
{
	CXXExecutor::CXXExecutor(HMODULE module, void* implementation) :
		implementation(implementation),
		initFunction(utility::load<InitExecutorSignature>(module, "webFrameworkCXXExecutorInit", true)),
		doPostFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoPost", true)),
		doGetFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoGet", true)),
		doHeadFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoHead", true)),
		doPutFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoPut", true)),
		doDeleteFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoDelete", true)),
		doPatchFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoPatch", true)),
		doOptionsFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoOptions", true)),
		doTraceFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoTrace", true)),
		doConnectFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoConnect", true)),
		getTypeFunction(utility::load<GetTypeSignature>(module, "webFrameworkCXXGetType", true)),
		destroyFunction(utility::load<DestroySignature>(module, "webFrameworkCXXDestroyExecutor", true)),
		deleteFunction(utility::load<DeleteSignature>(module, "webFrameworkCXXDeleteExecutor", true))
	{
		
	}

	void CXXExecutor::init(const utility::JSONSettingsParser::ExecutorSettings& settings)
	{
		initFunction(implementation, &settings);
	}

	void CXXExecutor::doPost(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		doPostFunction(implementation, &request, &response);
	}

	void CXXExecutor::doGet(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		doGetFunction(implementation, &request, &response);
	}

	void CXXExecutor::doHead(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		doHeadFunction(implementation, &request, &response);
	}

	void CXXExecutor::doPut(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		doPutFunction(implementation, &request, &response);
	}

	void CXXExecutor::doDelete(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		doDeleteFunction(implementation, &request, &response);
	}

	void CXXExecutor::doPatch(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		doPatchFunction(implementation, &request, &response);
	}

	void CXXExecutor::doOptions(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		doOptionsFunction(implementation, &request, &response);
	}

	void CXXExecutor::doTrace(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		doTraceFunction(implementation, &request, &response);
	}

	void CXXExecutor::doConnect(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		doConnectFunction(implementation, &request, &response);
	}

	utility::ExecutorType CXXExecutor::getType() const
	{
		return getTypeFunction(implementation);
	}

	void CXXExecutor::destroy()
	{
		utility::ExecutorType executorType = this->getType();

		if (executorType == utility::ExecutorType::stateful ||
			executorType == utility::ExecutorType::heavyOperationStateful)
		{
			destroyFunction(implementation);
		}
	}

	CXXExecutor::~CXXExecutor()
	{
		deleteFunction(implementation);
	}
}
