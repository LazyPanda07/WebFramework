#include "WebSocket/CCWebSocketExecutor.h"

namespace framework::web_socket
{
	CCWebSocketExecutor::CCWebSocketExecutor(HMODULE module, void* implementation, std::string_view executorName) :
		implementation(implementation),
		onReceiveFunction(utility::load<OnReceiveSignature>(module, std::format("webFrameworkCCWebSocketExecutorOnReceive{}", executorName), true)),
		deleteFunction(utility::load<DeleteSignature>(module, std::format("webFrameworkCCDeleteWebSocketExecutor{}", executorName), true))
	{

	}

	web::web_socket::Frame CCWebSocketExecutor::onReceive(const web::web_socket::Frame& frame)
	{
		web::web_socket::Frame data;

		onReceiveFunction(implementation, &const_cast<web::web_socket::Frame&>(frame), &WebSocketExecutor::defaultSendData, &data);

		return data;
	}

	CCWebSocketExecutor::~CCWebSocketExecutor()
	{
		deleteFunction(implementation);

		implementation = nullptr;
	}
}
