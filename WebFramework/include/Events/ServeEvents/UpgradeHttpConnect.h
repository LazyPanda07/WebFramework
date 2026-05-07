#pragma once

#include "ServeEvent.h"

#include "WebSocket/WebSocketExecutor.h"

namespace framework::event
{
	class UpgradeHttpConnect : public ServeEvent
	{
	private:
		std::unique_ptr<web_socket::WebSocketExecutor> executor;

	public:
		UpgradeHttpConnect(std::unique_ptr<web_socket::WebSocketExecutor>&& executor);

		void operator ()(streams::IOSocketStream& stream, std::unique_ptr<serve_loop::ServeLoop>& loop) override;

		~UpgradeHttpConnect() = default;
	};
}
