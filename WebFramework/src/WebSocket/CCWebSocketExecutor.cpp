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

		onReceiveFunction
		(
			implementation,
			&const_cast<web::web_socket::Frame&>(frame),
			[](const uint8_t* data, uint64_t size, int32_t type, void* additionalData)
			{
				*reinterpret_cast<web::web_socket::Frame*>(additionalData) = web::web_socket::Frame
				(
					true,
					static_cast<web::web_socket::Frame::OpcodeType>(type),
					std::string_view(reinterpret_cast<const char*>(data), size)
				);
			},
			&data
		);

		return data;
	}

	CCWebSocketExecutor::~CCWebSocketExecutor()
	{
		deleteFunction(implementation);

		implementation = nullptr;
	}
}
