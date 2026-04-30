#pragma once

#include <WebSocket/WebSocketExecutor.hpp>

namespace framework
{
	class PyWebSocketExecutor : public WebSocketExecutor
	{
	public:
		PyWebSocketExecutor() = default;

		std::variant<std::string, std::vector<uint8_t>> onReceive(const Frame& frame) override;

		virtual ~PyWebSocketExecutor() = default;
	};
}
