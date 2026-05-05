#pragma once

#include "ServeLoop.h"

#include <WebSocket/WebSocketExecutor.h>

namespace framework::serve_loop
{
	class WebSocketServeLoop : public ServeLoop
	{
	private:
		std::unique_ptr<web_socket::WebSocketExecutor> executor;

	private:
		bool run() override;

	public:
		WebSocketServeLoop(ServeLoop& serveLoop, std::unique_ptr<web_socket::WebSocketExecutor>&& executor);

		~WebSocketServeLoop() = default;
	};
}
