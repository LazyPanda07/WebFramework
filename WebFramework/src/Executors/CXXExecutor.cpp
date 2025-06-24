#include "CXXExecutor.h"

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

	}

	void CXXExecutor::doPost(HTTPRequest& request, HTTPResponse& response)
	{
		doPostFunction(implementation, request.getImplementation(), response.getImplementation());
	}

	void CXXExecutor::doGet(HTTPRequest& request, HTTPResponse& response)
	{
		doGetFunction(implementation, request.getImplementation(), response.getImplementation());
	}

	void CXXExecutor::doHead(HTTPRequest& request, HTTPResponse& response)
	{
		doHeadFunction(implementation, request.getImplementation(), response.getImplementation());
	}

	void CXXExecutor::doPut(HTTPRequest& request, HTTPResponse& response)
	{
		doPutFunction(implementation, request.getImplementation(), response.getImplementation());
	}

	void CXXExecutor::doDelete(HTTPRequest& request, HTTPResponse& response)
	{
		doDeleteFunction(implementation, request.getImplementation(), response.getImplementation());
	}

	void CXXExecutor::doPatch(HTTPRequest& request, HTTPResponse& response)
	{
		doPatchFunction(implementation, request.getImplementation(), response.getImplementation());
	}

	void CXXExecutor::doOptions(HTTPRequest& request, HTTPResponse& response)
	{
		doOptionsFunction(implementation, request.getImplementation(), response.getImplementation());
	}

	void CXXExecutor::doTrace(HTTPRequest& request, HTTPResponse& response)
	{
		doTraceFunction(implementation, request.getImplementation(), response.getImplementation());
	}

	void CXXExecutor::doConnect(HTTPRequest& request, HTTPResponse& response)
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
