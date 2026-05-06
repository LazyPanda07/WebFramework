#pragma once

#include "WebSocketExecutor.h"

#include "Utility/Sources.h"

namespace framework::web_socket
{
	class CXXWebSocketExecutor : public WebSocketExecutor
	{
	private:
		using OnReceiveSignature = void(*)(void* implementation, void* frame, void(*sendData)(const uint8_t* data, uint64_t size, int32_t type, void* additionalData), void* additionalData);
		using DeleteSignature = void(*)(void*);

	private:
		void* implementation;
		OnReceiveSignature onReceiveFunction;
		DeleteSignature deleteFunction;

	public:
		CXXWebSocketExecutor(HMODULE module, void* implementation);

		std::optional<web::web_socket::Frame> onReceive(const web::web_socket::Frame& frame) override;

		~CXXWebSocketExecutor();
	};
}
