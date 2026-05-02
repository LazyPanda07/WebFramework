#pragma once

#include "WebSocketExecutor.h"

#ifdef __WITH_PYTHON_EXECUTORS__

namespace framework::web_socket
{
	class PythonWebSocketExecutor : public WebSocketExecutor
	{
	public:
		PythonWebSocketExecutor();

		~PythonWebSocketExecutor() = default;
	};
}

#endif
