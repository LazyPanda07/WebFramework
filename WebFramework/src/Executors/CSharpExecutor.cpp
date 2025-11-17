#include "CSharpExecutor.h"

#ifdef __WITH_DOT_NET_EXECUTORS__

#include "Managers/RuntimesManager.h"

namespace framework
{
	CSharpExecutor::CSharpExecutor(void* implementation, const std::filesystem::path& modulePath) :
		implementation(implementation),
		moduleName(runtime::DotNetRuntime::getModuleName(modulePath))
	{

	}

	void CSharpExecutor::init(const utility::JSONSettingsParser::ExecutorSettings& settings)
	{

	}

	void CSharpExecutor::doGet(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();

		void* dotNetRequest = runtime.createHTTPRequest(request.getImplementation());
		void* dotNetResponse = runtime.createHTTPResponse(response.getImplementation());

		if (!runtime[moduleName].doGet(implementation, dotNetRequest, dotNetResponse))
		{
			throw std::runtime_error("Exception while calling DoGet");
		}
	}

	void CSharpExecutor::doPost(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void CSharpExecutor::doHead(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void CSharpExecutor::doPut(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void CSharpExecutor::doDelete(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void CSharpExecutor::doPatch(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void CSharpExecutor::doOptions(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void CSharpExecutor::doTrace(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	void CSharpExecutor::doConnect(HTTPRequestExecutors& request, HTTPResponseExecutors& response)
	{

	}

	utility::ExecutorType CSharpExecutor::getType() const
	{
		return utility::ExecutorType::stateless;
	}

	void CSharpExecutor::destroy()
	{

	}

	CSharpExecutor::~CSharpExecutor()
	{

	}
}

#endif
