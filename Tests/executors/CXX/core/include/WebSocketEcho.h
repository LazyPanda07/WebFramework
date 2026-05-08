#pragma once

#include <WebSocket/WebSocketExecutor.hpp>

class WebSocketEcho : public framework::WebSocketExecutor
{
public:
	WebSocketEcho() = default;

	std::optional<std::variant<std::string, std::vector<uint8_t>>> onReceive(const framework::WebSocketExecutor::Frame& frame, std::optional<framework::WebSocketExecutor::Frame::Close>& close) override;

	~WebSocketEcho() = default;
};
