#include "WebSocket/PythonWebSocketExecutor.h"

#include <format>

#ifdef __WITH_PYTHON_EXECUTORS__

#include "Managers/RuntimesManager.h"
#include "Runtimes/PythonRuntime.h"

namespace py = pybind11;

namespace framework::web_socket
{
	PythonWebSocketExecutor::PythonWebSocketExecutor(pybind11::object* implementation) :
		implementation(implementation)
	{

	}

	web::web_socket::Frame PythonWebSocketExecutor::onReceive(const web::web_socket::Frame& frame)
	{
		py::gil_scoped_acquire gil;
		const runtime::PythonRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::PythonRuntime>();
		std::unique_ptr<py::object> frameWrapper(runtime.createWebSocketFrame(&frame));

		std::variant<std::string, py::bytes> result = implementation->attr("on_receive")(*frameWrapper).cast<std::variant<std::string, py::bytes>>();
		std::string_view data;
		web::web_socket::Frame::OpcodeType type;

		if (std::holds_alternative<std::string>(result))
		{
			data = std::get<std::string>(result);
			type = web::web_socket::Frame::OpcodeType::text;
		}
		else if (std::holds_alternative<py::bytes>(result))
		{
			data = std::get<py::bytes>(result);
			type = web::web_socket::Frame::OpcodeType::binary;
		}
		else
		{
			throw std::runtime_error(std::format("Wrong return type: {}", result.index()));
		}

		return web::web_socket::Frame(true, type, data);
	}

	PythonWebSocketExecutor::~PythonWebSocketExecutor()
	{
		if (implementation)
		{
			py::gil_scoped_acquire gil;

			delete implementation;

			implementation = nullptr;
		}
	}
}

#endif
