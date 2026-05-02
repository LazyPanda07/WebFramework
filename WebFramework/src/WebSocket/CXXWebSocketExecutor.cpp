#include "WebSocket/CXXWebSocketExecutor.h"

namespace framework::web_socket
{
	CXXWebSocketExecutor::CXXWebSocketExecutor(HMODULE module, void* implementation) :
		implementation(implementation),
		onReceiveFunction(utility::load<OnReceiveSignature>(module, "webFrameworkCXXWebSocketExecutorOnReceive", true)),
		deleteFunction(utility::load<DeleteSignature>(module, "webFrameworkCXXDeleteWebSocketExecutor", true))
	{

	}

	web::web_socket::Frame CXXWebSocketExecutor::onReceive(const web::web_socket::Frame& frame)
	{
		web::web_socket::Frame data;

		onReceiveFunction(implementation, &const_cast<web::web_socket::Frame&>(frame), &WebSocketExecutor::defaultSendData, &data);

		return data;
	}

	CXXWebSocketExecutor::~CXXWebSocketExecutor()
	{
		deleteFunction(implementation);

		implementation = nullptr;
	}
}
