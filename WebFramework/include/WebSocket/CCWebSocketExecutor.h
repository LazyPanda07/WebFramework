#pragma once

#include "WebSocketExecutor.h"

#include "Utility/Sources.h"

namespace framework::web_socket
{
	class CCWebSocketExecutor : public WebSocketExecutor
	{
	private:
		using OnReceiveSignature = void(*)(void* implementation, void* frame, void(*send_data)(const uint8_t* data, uint64_t size, int32_t type, void* additionalData), void* additionalData);
		using DeleteSignature = void(*)(void* implementation);

	private:
		void* implementation;
		OnReceiveSignature onReceiveFunction;
		DeleteSignature deleteFunction;

	public:
		CCWebSocketExecutor(HMODULE module, void* implementation, std::string_view executorName);

		web::web_socket::Frame onReceive(const web::web_socket::Frame& frame) override;

		~CCWebSocketExecutor();
	};
}
