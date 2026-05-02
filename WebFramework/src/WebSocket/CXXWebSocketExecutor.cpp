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

	CXXWebSocketExecutor::~CXXWebSocketExecutor()
	{
		deleteFunction(implementation);

		implementation = nullptr;
	}
}
