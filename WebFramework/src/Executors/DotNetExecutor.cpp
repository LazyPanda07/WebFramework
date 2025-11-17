#include "DotNetExecutor.h"

#ifdef __WITH_DOT_NET_EXECUTORS__

#include "Managers/RuntimesManager.h"

namespace framework
{
	DotNetExecutor::DotNetExecutor(void* implementation, const std::filesystem::path& modulePath) :
		implementation(implementation),
		moduleName(runtime::DotNetRuntime::getModuleName(modulePath))
	{

	}

	void DotNetExecutor::init(const utility::JSONSettingsParser::ExecutorSettings& settings)
	{

	}

	void DotNetExecutor::doGet(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		void* dotNetRequest = runtime.createHTTPRequest(request.getImplementation());
		void* dotNetResponse = runtime.createHTTPResponse(response.getImplementation());

		if (!runtime[moduleName].doGet(implementation, dotNetRequest, dotNetResponse))
		{
			throw std::runtime_error("Exception while calling DoGet");
		}
	}

	void DotNetExecutor::doPost(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void DotNetExecutor::doHead(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void DotNetExecutor::doPut(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void DotNetExecutor::doDelete(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void DotNetExecutor::doPatch(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void DotNetExecutor::doOptions(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void DotNetExecutor::doTrace(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void DotNetExecutor::doConnect(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	utility::ExecutorType DotNetExecutor::getType() const
	{
		return utility::ExecutorType::stateless;
	}

	void DotNetExecutor::destroy()
	{

	}

	DotNetExecutor::~DotNetExecutor()
	{

	}
}

#endif
