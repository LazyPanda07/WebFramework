#include "WebSocket/PyWebSocketExecutor.h"

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace framework
{
	std::variant<std::string, std::vector<uint8_t>> PyWebSocketExecutor::onReceive(const Frame& frame)
	{
		using ReturnType = std::variant<std::string, std::vector<uint8_t>>;

		PYBIND11_OVERRIDE_PURE
		(
			ReturnType,
			WebSocketExecutor,
			onReceive,
			frame
		);
	}
}
