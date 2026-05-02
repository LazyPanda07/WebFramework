#include "WebSocket/WebSocketExecutor.h"

namespace framework::web_socket
{
	void WebSocketExecutor::defaultSendData(const uint8_t* data, uint64_t size, int32_t type, void* additionalData)
	{
		*reinterpret_cast<web::web_socket::Frame*>(additionalData) = web::web_socket::Frame
		(
			true,
			static_cast<web::web_socket::Frame::OpcodeType>(type),
			std::string_view(reinterpret_cast<const char*>(data), size)
		);
	}
}
