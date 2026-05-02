#pragma once

#include "WebSocketExecutor.h"

#ifdef __WITH_DOTNET_EXECUTORS__

namespace framework::web_socket
{
	class CSharpWebSocketExecutor : public WebSocketExecutor
	{
	private:
		void* implementation;

	public:
		CSharpWebSocketExecutor(void* implementation);

		web::web_socket::Frame onReceive(const web::web_socket::Frame& frame) override;

		~CSharpWebSocketExecutor();
	};
}

#endif
