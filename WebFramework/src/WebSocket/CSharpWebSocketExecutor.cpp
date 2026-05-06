#include "WebSocket/CSharpWebSocketExecutor.h"

#ifdef __WITH_DOTNET_EXECUTORS__

#include "Managers/RuntimesManager.h"
#include "Runtimes/DotNetRuntime.h"

namespace framework::web_socket
{
	CSharpWebSocketExecutor::CSharpWebSocketExecutor(void* implementation) :
		implementation(implementation)
	{

	}

	std::optional<web::web_socket::Frame> CSharpWebSocketExecutor::onReceive(const web::web_socket::Frame& frame)
	{
		runtime::DotNetRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>();
		web::web_socket::Frame data;

		runtime.getCallOnReceive()(implementation, &const_cast<web::web_socket::Frame&>(frame), &WebSocketExecutor::defaultSendData, &data);

		return data;
	}

	CSharpWebSocketExecutor::~CSharpWebSocketExecutor()
	{
		if (implementation)
		{
			runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>().free(implementation);

			implementation = nullptr;
		}
	}
}

#endif
