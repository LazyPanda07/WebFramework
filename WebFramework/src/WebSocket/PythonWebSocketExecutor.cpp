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

	std::optional<web::web_socket::Frame> PythonWebSocketExecutor::onReceive(const web::web_socket::Frame& frame)
	{
		py::gil_scoped_acquire gil;
		const runtime::PythonRuntime& runtime = runtime::RuntimesManager::get().getRuntime<runtime::PythonRuntime>();
		std::unique_ptr<py::object> frameWrapper(runtime.createWebSocketFrame(&frame));

		py::object result = implementation->attr("on_receive")(*frameWrapper);

		if (result.is_none())
		{
			return std::nullopt;
		}

		std::string data;
		web::web_socket::Frame::OpcodeType type;

		if (py::isinstance<py::str>(result))
		{
			data = result.cast<py::str>();
			type = web::web_socket::Frame::OpcodeType::text;
		}
		else if (py::isinstance<py::bytes>(result))
		{
			data = result.cast<py::bytes>();
			type = web::web_socket::Frame::OpcodeType::binary;
		}
		else
		{
			throw std::runtime_error(std::format("Wrong type: {}", py::repr(result).cast<std::string>()));
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
