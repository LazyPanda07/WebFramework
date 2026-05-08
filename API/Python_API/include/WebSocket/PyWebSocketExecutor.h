#pragma once

#include <WebSocket/WebSocketExecutor.hpp>

#include <pybind11/stl.h>

namespace framework
{
	class PyWebSocketExecutor : public WebSocketExecutor
	{
	public:
		PyWebSocketExecutor() = default;

		std::optional<std::variant<std::string, std::vector<uint8_t>>> onReceive(const Frame& frame, std::optional<framework::WebSocketExecutor::Frame::Close>& close) override;

		virtual ~PyWebSocketExecutor() = default;
	};
}
