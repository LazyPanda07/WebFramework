#include "CXXExecutor.h"

#include <format>

#include <Log.h>

#define ASSERT_LOAD_FUNCTION(name) if (!static_cast<bool>(name)) throw std::runtime_error(std::format("Can't load {} function", #name))

namespace framework
{
	CXXExecutor::CXXExecutor(HMODULE module, void* implementation) :
		implementation(implementation),
		initFunction(utility::load<InitExecutorSignature>(module, "webFrameworkCXXExecutorInit")),
		doPostFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoPost")),
		doGetFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoGet")),
		doHeadFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoHead")),
		doPutFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoPut")),
		doDeleteFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoDelete")),
		doPatchFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoPatch")),
		doOptionsFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoOptions")),
		doTraceFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoTrace")),
		doConnectFunction(utility::load<DoMethodSignature>(module, "webFrameworkCXXDoConnect")),
		getTypeFunction(utility::load<GetTypeSignature>(module, "webFrameworkCXXGetType")),
		destroyFunction(utility::load<DestroySignature>(module, "webFrameworkCXXDestroyExecutor")),
		deleteFunction(utility::load<DeleteSignature>(module, "webFrameworkCXXDeleteExecutor"))
	{
		ASSERT_LOAD_FUNCTION(initFunction);
		ASSERT_LOAD_FUNCTION(doPostFunction);
		ASSERT_LOAD_FUNCTION(doGetFunction);
		ASSERT_LOAD_FUNCTION(doHeadFunction);
		ASSERT_LOAD_FUNCTION(doPutFunction);
		ASSERT_LOAD_FUNCTION(doDeleteFunction);
		ASSERT_LOAD_FUNCTION(doPatchFunction);
		ASSERT_LOAD_FUNCTION(doOptionsFunction);
		ASSERT_LOAD_FUNCTION(doTraceFunction);
		ASSERT_LOAD_FUNCTION(doConnectFunction);
		ASSERT_LOAD_FUNCTION(getTypeFunction);
		ASSERT_LOAD_FUNCTION(destroyFunction);
		ASSERT_LOAD_FUNCTION(deleteFunction);
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
