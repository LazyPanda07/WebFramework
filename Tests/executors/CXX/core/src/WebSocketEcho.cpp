#include "WebSocketEcho.h"

std::optional<std::variant<std::string, std::vector<uint8_t>>> WebSocketEcho::onReceive(const framework::WebSocketExecutor::Frame& frame, std::optional<framework::WebSocketExecutor::Frame::Close>& close)
{
	return std::string(frame.getPayload());
}

DEFINE_WEB_SOCKET_EXECUTOR(WebSocketEcho)
