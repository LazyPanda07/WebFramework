#include "CSharpExecutor.h"

#ifdef __WITH_DOT_NET_EXECUTORS__

#include "Managers/RuntimesManager.h"
#include "WebInterfaces/IHTTPRequest.h"
#include "Exceptions/CSharpException.h"
#include "Runtimes/DotNetRuntime.h"

struct Deleter
{
	void operator ()(void* implementation) const noexcept;
};

namespace framework
{
	void CSharpExecutor::processMethod(runtime::Runtime& runtime, runtime::DotNetRuntime::DoMethodSignature method, HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		std::unique_ptr<void, Deleter> dotNetRequest(runtime.createHTTPRequest(request.getImplementation()));
		std::unique_ptr<void, Deleter> dotNetResponse(runtime.createHTTPResponse(response.getImplementation()));
		framework::interfaces::CExceptionData exceptionData;

		bool success = method(implementation, dotNetRequest.get(), dotNetResponse.get());

		if (!success)
		{
			static_cast<interfaces::IHTTPRequest*>(request.getImplementation())->getExceptionData(&exceptionData);

			throw exceptions::CSharpException(exceptionData.errorMessage, static_cast<web::ResponseCodes>(exceptionData.responseCode), exceptionData.logCategory);
		}
	}

	CSharpExecutor::CSharpExecutor(void* implementation) :
		implementation(implementation)
	{

	}

	void CSharpExecutor::init(const utility::JSONSettingsParser::ExecutorSettings& settings)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();
		std::unique_ptr<void, Deleter> dotNetExecutorSettings(runtime.createExecutorSettings(&settings));

		runtime.getInit()(implementation, dotNetExecutorSettings.get());
	}

	void CSharpExecutor::doGet(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.getDoGet(), request, response);
	}

	void CSharpExecutor::doPost(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.getDoPost(), request, response);
	}

	void CSharpExecutor::doHead(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.getDoHead(), request, response);
	}

	void CSharpExecutor::doPut(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.getDoPut(), request, response);
	}

	void CSharpExecutor::doDelete(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.getDoDelete(), request, response);
	}

	void CSharpExecutor::doPatch(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.getDoPatch(), request, response);
	}

	void CSharpExecutor::doOptions(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.getDoOptions(), request, response);
	}

	void CSharpExecutor::doTrace(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.getDoTrace(), request, response);
	}

	void CSharpExecutor::doConnect(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.getDoConnect(), request, response);
	}

	utility::ExecutorType CSharpExecutor::getType() const
	{
		return static_cast<utility::ExecutorType>(runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>().getGetExecutorType()(implementation));
	}

	void CSharpExecutor::destroy()
	{
		runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>().getDestroy()(implementation);
	}

	CSharpExecutor::~CSharpExecutor()
	{
		if (!implementation)
		{
			return;
		}

		runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>().free(implementation);

		implementation = nullptr;
	}
}

void Deleter::operator ()(void* implementation) const noexcept
{
	framework::runtime::RuntimesManager::get().getRuntime<framework::runtime::DotNetRuntime>().free(implementation);
}

#endif
