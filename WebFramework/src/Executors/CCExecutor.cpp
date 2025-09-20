#include "CCExecutor.h"

#include "Exceptions/NotImplementedException.h"

namespace framework
{
	template<typename T, typename... Args>
	void CCExecutor::callBindedMethodFunction(const T& function, std::string_view functionName, Args&&... args) requires std::invocable<T, Args...>
	{
		if (function)
		{
			function(std::forward<Args>(args)...);
		}
		else
		{
			throw exceptions::NotImplementedException(executorName, functionName);
		}
	}

	CCExecutor::CCExecutor(HMODULE module, void* implementation, std::string_view executorName) :
		implementation(implementation),
		executorName(executorName),
		initFunction(utility::load<InitExecutorSignature>(module, std::format("webFrameworkCCExecutorInit{}", executorName))),
		doPostFunction(utility::load<DoMethodSignature>(module, std::format("webFrameworkCCDoPOST_METHOD{}", executorName))),
		doGetFunction(utility::load<DoMethodSignature>(module, std::format("webFrameworkCCDoGET_METHOD{}", executorName))),
		doHeadFunction(utility::load<DoMethodSignature>(module, std::format("webFrameworkCCDoHEAD_METHOD{}", executorName))),
		doPutFunction(utility::load<DoMethodSignature>(module, std::format("webFrameworkCCDoPUT_METHOD{}", executorName))),
		doDeleteFunction(utility::load<DoMethodSignature>(module, std::format("webFrameworkCCDoDELETE_METHOD{}", executorName))),
		doPatchFunction(utility::load<DoMethodSignature>(module, std::format("webFrameworkCCDoPATCH_METHOD{}", executorName))),
		doOptionsFunction(utility::load<DoMethodSignature>(module, std::format("webFrameworkCCDoOPTIONS_METHOD{}", executorName))),
		doTraceFunction(utility::load<DoMethodSignature>(module, std::format("webFrameworkCCDoTRACE_METHOD{}", executorName))),
		doConnectFunction(utility::load<DoMethodSignature>(module, std::format("webFrameworkCCDoCONNECT_METHOD{}", executorName))),
		getTypeFunction(utility::load<GetTypeSignature>(module, std::format("webFrameworkCCGetType{}", executorName))),
		destroyFunction(utility::load<DestroySignature>(module, std::format("webFrameworkCCDestroyExecutor{}", executorName))),
		deleteFunction(utility::load<DeleteSignature>(module, std::format("webFrameworkCCDeleteExecutor{}", executorName)))
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
		this->callBindedMethodFunction(doPostFunction, __func__, implementation, request.getImplementation(), response.getImplementation());
	}

	void CCExecutor::doGet(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->callBindedMethodFunction(doGetFunction, __func__, implementation, request.getImplementation(), response.getImplementation());
	}

	void CCExecutor::doHead(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->callBindedMethodFunction(doHeadFunction, __func__, implementation, request.getImplementation(), response.getImplementation());
	}

	void CCExecutor::doPut(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->callBindedMethodFunction(doPutFunction, __func__, implementation, request.getImplementation(), response.getImplementation());
	}

	void CCExecutor::doDelete(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->callBindedMethodFunction(doDeleteFunction, __func__, implementation, request.getImplementation(), response.getImplementation());
	}

	void CCExecutor::doPatch(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->callBindedMethodFunction(doPatchFunction, __func__, implementation, request.getImplementation(), response.getImplementation());
	}

	void CCExecutor::doOptions(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		if (doOptionsFunction)
		{
			doOptionsFunction(implementation, request.getImplementation(), response.getImplementation());
		}
		else
		{
			BaseExecutor::doOptions(request, response);
		}
	}

	void CCExecutor::doTrace(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		if (doTraceFunction)
		{
			doTraceFunction(implementation, request.getImplementation(), response.getImplementation());
		}
		else
		{
			BaseExecutor::doTrace(request, response);
		}
	}

	void CCExecutor::doConnect(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		this->callBindedMethodFunction(doConnectFunction, __func__, implementation, request.getImplementation(), response.getImplementation());
	}

	utility::ExecutorType CCExecutor::getType() const
	{
		return static_cast<utility::ExecutorType>(getTypeFunction(implementation));
	}

	void CCExecutor::destroy()
	{
		utility::ExecutorType executorType = this->getType();

		if (executorType == utility::ExecutorType::stateful ||
			executorType == utility::ExecutorType::heavyOperationStateful)
		{
			destroyFunction(implementation);
		}
	}

	CCExecutor::~CCExecutor()
	{
		this->destroy();

		deleteFunction(implementation);
	}
}
