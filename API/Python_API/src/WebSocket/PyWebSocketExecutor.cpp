#include "WebSocket/PyWebSocketExecutor.h"

#include <pybind11/pybind11.h>

namespace framework
{
	std::optional<std::variant<std::string, std::vector<uint8_t>>> PyWebSocketExecutor::onReceive(const Frame& frame)
	{
		using ReturnType = std::optional<std::variant<std::string, std::vector<uint8_t>>>;

		PYBIND11_OVERRIDE_PURE
		(
			ReturnType,
			WebSocketExecutor,
			onReceive,
			frame
		);
	}
}
