#include "Executors/CSharpExecutor.h"

#ifdef __WITH_DOTNET_EXECUTORS__

#include "Managers/RuntimesManager.h"
#include "WebInterfaces/IHttpRequest.h"
#include "Exceptions/CSharpException.h"
#include "Runtimes/DotNetRuntime.h"

struct Deleter
{
	void operator ()(void* implementation) const noexcept;
};

namespace framework
{
	void CSharpExecutor::processMethod(runtime::Runtime& runtime, runtime::DotNetRuntime::DoMethodSignature method, interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		std::unique_ptr<void, Deleter> dotNetRequest(runtime.createHTTPRequest(&request));
		std::unique_ptr<void, Deleter> dotNetResponse(runtime.createHTTPResponse(&response));
		framework::interfaces::CExceptionData exceptionData;

		bool success = method(implementation, dotNetRequest.get(), dotNetResponse.get());

		if (!success)
		{
			request.getExceptionData(&exceptionData);

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

	void CSharpExecutor::doGet(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.getDoGet(), request, response);
	}

	void CSharpExecutor::doPost(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.getDoPost(), request, response);
	}

	void CSharpExecutor::doHead(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.getDoHead(), request, response);
	}

	void CSharpExecutor::doPut(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.getDoPut(), request, response);
	}

	void CSharpExecutor::doDelete(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.getDoDelete(), request, response);
	}

	void CSharpExecutor::doPatch(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.getDoPatch(), request, response);
	}

	void CSharpExecutor::doOptions(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.getDoOptions(), request, response);
	}

	void CSharpExecutor::doTrace(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.getDoTrace(), request, response);
	}

	void CSharpExecutor::doConnect(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response)
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
		utility::ExecutorType executorType = this->getType();

		if (executorType == utility::ExecutorType::stateful ||
			executorType == utility::ExecutorType::heavyOperationStateful)
		{
			runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>().getDestroy()(implementation);
		}
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
