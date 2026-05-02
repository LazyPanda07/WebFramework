#pragma once

#include <string>

#include <WebSocket/Frame.h>

namespace framework::web_socket
{
	class WebSocketExecutor
	{
	protected:
		static void defaultSendData(const uint8_t* data, uint64_t size, int32_t type, void* additionalData);

	public:
		WebSocketExecutor() = default;

		virtual web::web_socket::Frame onReceive(const web::web_socket::Frame& frame) = 0;

		virtual ~WebSocketExecutor() = default;
	};
}
