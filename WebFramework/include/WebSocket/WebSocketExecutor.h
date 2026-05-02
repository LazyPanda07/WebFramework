#pragma once

#include <string>

#include <WebSocket/Frame.h>

namespace framework::web_socket
{
	class WebSocketExecutor
	{
	public:
		WebSocketExecutor() = default;

		virtual web::web_socket::Frame onReceive(const web::web_socket::Frame& frame) = 0;

		virtual ~WebSocketExecutor() = default;
	};
}
