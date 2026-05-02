#pragma once

#include "WebSocketExecutor.h"

#ifdef __WITH_DOTNET_EXECUTORS__

namespace framework::web_socket
{
	class CSharpWebSocketExecutor : public WebSocketExecutor
	{
	public:
		CSharpWebSocketExecutor();

		~CSharpWebSocketExecutor() = default;
	};
}

#endif
