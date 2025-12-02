#include "CSharpExecutor.h"

#ifdef __WITH_DOT_NET_EXECUTORS__

#include "Managers/RuntimesManager.h"
#include "WebInterfaces/IHTTPRequest.h"
#include "Exceptions/CSharpException.h"

struct Deleter
{
	void operator ()(void* implementation) const noexcept;
};

namespace framework
{
	void CSharpExecutor::processMethod(runtime::DotNetRuntime& runtime, runtime::DotNetRuntime::DoMethodSignature method, HTTPRequestExecutors& request, HTTPResponseExecutors& response)
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

	CSharpExecutor::CSharpExecutor(void* implementation, const std::filesystem::path& modulePath) :
		implementation(implementation),
		moduleName(runtime::DotNetRuntime::getModuleName(modulePath))
	{

	}

	void CSharpExecutor::init(const utility::JSONSettingsParser::ExecutorSettings& settings)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();
		std::unique_ptr<void, Deleter> dotNetExecutorSettings(runtime.createExecutorSettings(&settings));

		runtime.init(implementation, dotNetExecutorSettings.get());
	}

	void CSharpExecutor::doGet(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.doGet, request, response);
	}

	void CSharpExecutor::doPost(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.doPost, request, response);
	}

	void CSharpExecutor::doHead(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.doHead, request, response);
	}

	void CSharpExecutor::doPut(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.doPut, request, response);
	}

	void CSharpExecutor::doDelete(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.doDelete, request, response);
	}

	void CSharpExecutor::doPatch(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.doPatch, request, response);
	}

	void CSharpExecutor::doOptions(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.doOptions, request, response);
	}

	void CSharpExecutor::doTrace(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.doTrace, request, response);
	}

	void CSharpExecutor::doConnect(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		this->processMethod(runtime, runtime.doConnect, request, response);
	}

	utility::ExecutorType CSharpExecutor::getType() const
	{
		return static_cast<utility::ExecutorType>(runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>().getExecutorType(implementation));
	}

	void CSharpExecutor::destroy()
	{
		runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>().destroy(implementation);
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
