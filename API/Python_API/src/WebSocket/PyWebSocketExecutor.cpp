#include "WebSocket/PyWebSocketExecutor.h"

#include <pybind11/pybind11.h>

namespace framework
{
	std::optional<std::variant<std::string, std::vector<uint8_t>>> PyWebSocketExecutor::onReceive(const Frame& frame, std::optional<framework::WebSocketExecutor::Frame::Close>& close)
	{
		using ReturnType = std::optional<std::variant<std::string, std::vector<uint8_t>>>;
		using CloseType = std::optional<framework::WebSocketExecutor::Frame::Close>;

		PYBIND11_OVERRIDE_PURE
		(
			ReturnType,
			WebSocketExecutor,
			onReceive,
			frame,
			close
		);
	}
}
