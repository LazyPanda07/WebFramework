#include "CXXExecutor.h"

#include <format>

#include "Log.h"

#define ASSERT_LOAD_FUNCTION(name) if (!static_cast<bool>(name)) throw runtime_error(format("Can't load {} function", #name))

using namespace std;

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
		try
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
		catch (const exception& e)
		{
			Log::fatalError("Load function error: {}", "LogCXXExecutor", 1, e.what());
		}
	}

	void CXXExecutor::init(const utility::JSONSettingsParser::ExecutorSettings& settings)
	{
		initFunction(implementation, &settings);
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

	CXXExecutor::~CXXExecutor()
	{
		this->destroy();

		deleteFunction(implementation);
	}
}
