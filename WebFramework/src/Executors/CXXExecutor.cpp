#include "CXXExecutor.h"

#include <format>

#define ASSERT_LOAD_FUNCTION(name) if (!static_cast<bool>(name)) throw runtime_error(format("Can't load {} function", #name))

using namespace std;

namespace framework
{
	CXXExecutor::CXXExecutor(HMODULE module, void* implementation) :
		implementation(implementation),
		doPostFunction(utility::load<DoMethodSignature>(module, "webFrameworkDoPost")),
		doGetFunction(utility::load<DoMethodSignature>(module, "webFrameworkDoGet")),
		doHeadFunction(utility::load<DoMethodSignature>(module, "webFrameworkDoHead")),
		doPutFunction(utility::load<DoMethodSignature>(module, "webFrameworkDoPut")),
		doDeleteFunction(utility::load<DoMethodSignature>(module, "webFrameworkDoDelete")),
		doPatchFunction(utility::load<DoMethodSignature>(module, "webFrameworkDoPatch")),
		doOptionsFunction(utility::load<DoMethodSignature>(module, "webFrameworkDoOptions")),
		doTraceFunction(utility::load<DoMethodSignature>(module, "webFrameworkDoTrace")),
		doConnectFunction(utility::load<DoMethodSignature>(module, "webFrameworkDoConnect")),
		getTypeFunction(utility::load<GetTypeSignature>(module, "webFrameworkGetType")),
		destroyFunction(utility::load<DestroySignature>(module, "webFrameworkDestroyExecutor"))
	{
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
	}

	void CXXExecutor::doPost(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		doPostFunction(implementation, request.getImplementation(), response.getImplementation());
	}

	void CXXExecutor::doGet(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		doGetFunction(implementation, request.getImplementation(), response.getImplementation());
	}

	void CXXExecutor::doHead(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		doHeadFunction(implementation, request.getImplementation(), response.getImplementation());
	}

	void CXXExecutor::doPut(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		doPutFunction(implementation, request.getImplementation(), response.getImplementation());
	}

	void CXXExecutor::doDelete(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		doDeleteFunction(implementation, request.getImplementation(), response.getImplementation());
	}

	void CXXExecutor::doPatch(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		doPatchFunction(implementation, request.getImplementation(), response.getImplementation());
	}

	void CXXExecutor::doOptions(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		doOptionsFunction(implementation, request.getImplementation(), response.getImplementation());
	}

	void CXXExecutor::doTrace(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		doTraceFunction(implementation, request.getImplementation(), response.getImplementation());
	}

	void CXXExecutor::doConnect(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		doConnectFunction(implementation, request.getImplementation(), response.getImplementation());
	}

	utility::ExecutorType CXXExecutor::getType() const
	{
		return getTypeFunction(implementation);
	}

	void CXXExecutor::destroy()
	{
		destroyFunction(implementation);
	}
}
