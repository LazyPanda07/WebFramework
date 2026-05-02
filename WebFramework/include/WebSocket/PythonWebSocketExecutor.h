#pragma once

#include "WebSocketExecutor.h"

#ifdef __WITH_PYTHON_EXECUTORS__

#include <pybind11/embed.h>

namespace framework::web_socket
{
	class PythonWebSocketExecutor : public WebSocketExecutor
	{
	private:
		pybind11::object* implementation;

	public:
		PythonWebSocketExecutor(pybind11::object* implementation);

		web::web_socket::Frame onReceive(const web::web_socket::Frame& frame) override;

		~PythonWebSocketExecutor();
	};
}

#endif
