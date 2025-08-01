#include "CCExecutor.h"

#include "Exceptions/NotImplementedException.h"

using namespace std;

namespace framework
{
	template<typename T, typename... Args>
	void CCExecutor::callBindedFunction(const T& function, string_view functionName, Args&&... args) requires invocable<T, Args...>
	{
		if (function)
		{
			function(forward<Args>(args)...);
		}
		else
		{
			throw exceptions::NotImplementedException(executorName, functionName);
		}
	}

	CCExecutor::CCExecutor(HMODULE module, void* implementation, string_view executorName) :
		implementation(implementation),
		executorName(executorName),
		initFunction(utility::load<InitExecutorSignature>(module, format("webFrameworkCCExecutorInit{}", executorName))),
		doPostFunction(utility::load<DoMethodSignature>(module, format("webFrameworkCCDoPost{}", executorName))),
		doGetFunction(utility::load<DoMethodSignature>(module, format("webFrameworkCCDoGet{}", executorName))),
		doHeadFunction(utility::load<DoMethodSignature>(module, format("webFrameworkCCDoHead{}", executorName))),
		doPutFunction(utility::load<DoMethodSignature>(module, format("webFrameworkCCDoPut{}", executorName))),
		doDeleteFunction(utility::load<DoMethodSignature>(module, format("webFrameworkCCDoDelete{}", executorName))),
		doPatchFunction(utility::load<DoMethodSignature>(module, format("webFrameworkCCDoPatch{}", executorName))),
		doOptionsFunction(utility::load<DoMethodSignature>(module, format("webFrameworkCCDoOptions{}", executorName))),
		doTraceFunction(utility::load<DoMethodSignature>(module, format("webFrameworkCCDoTrace{}", executorName))),
		doConnectFunction(utility::load<DoMethodSignature>(module, format("webFrameworkCCDoConnect{}", executorName))),
		getTypeFunction(utility::load<GetTypeSignature>(module, format("webFrameworkCCGetType{}", executorName))),
		destroyFunction(utility::load<DestroySignature>(module, format("webFrameworkCCDestroyExecutor{}", executorName))),
		deleteFunction(utility::load<DeleteSignature>(module, format("webFrameworkCCDeleteExecutor{}", executorName)))
	{
		if (!initFunction)
		{
			initFunction = [](void*, const void*) {};
		}

		if (!destroyFunction)
		{
			destroyFunction = [](void*) {};
		}
	}

	void CCExecutor::init(const utility::JSONSettingsParser::ExecutorSettings& settings)
	{
		initFunction(implementation, &settings);
	}

	void CCExecutor::doPost(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->callBindedFunction(doPostFunction, __func__, implementation, request.getImplementation(), response.getImplementation());
	}

	void CCExecutor::doGet(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->callBindedFunction(doGetFunction, __func__, implementation, request.getImplementation(), response.getImplementation());
	}

	void CCExecutor::doHead(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->callBindedFunction(doHeadFunction, __func__, implementation, request.getImplementation(), response.getImplementation());
	}

	void CCExecutor::doPut(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->callBindedFunction(doPutFunction, __func__, implementation, request.getImplementation(), response.getImplementation());
	}

	void CCExecutor::doDelete(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->callBindedFunction(doDeleteFunction, __func__, implementation, request.getImplementation(), response.getImplementation());
	}

	void CCExecutor::doPatch(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->callBindedFunction(doPatchFunction, __func__, implementation, request.getImplementation(), response.getImplementation());
	}

	void CCExecutor::doOptions(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->callBindedFunction(doOptionsFunction, __func__, implementation, request.getImplementation(), response.getImplementation());
	}

	void CCExecutor::doTrace(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->callBindedFunction(doTraceFunction, __func__, implementation, request.getImplementation(), response.getImplementation());
	}

	void CCExecutor::doConnect(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->callBindedFunction(doConnectFunction, __func__, implementation, request.getImplementation(), response.getImplementation());
	}

	utility::ExecutorType CCExecutor::getType() const
	{
		return static_cast<utility::ExecutorType>(getTypeFunction(implementation));
	}

	void CCExecutor::destroy()
	{
		destroyFunction(implementation);
	}

	CCExecutor::~CCExecutor()
	{
		this->destroy();

		this->callBindedFunction(deleteFunction, __func__, implementation);
	}
}
