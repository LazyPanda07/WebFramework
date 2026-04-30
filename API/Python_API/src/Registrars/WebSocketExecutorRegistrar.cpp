#include "Registrars/WebSocketExecutorRegistrar.h"

#include <pybind11/stl.h>

#include "WebSocket/PyWebSocketExecutor.h"

namespace py = pybind11;

namespace registrar
{
	void registerWebSocketExecutor(pybind11::module_& m)
	{
		using namespace py::literals;

		py::class_<framework::WebSocketExecutor::Frame> frameClass(m, "Frame");

		py::enum_<framework::WebSocketExecutor::Frame::Type>(frameClass, "FrameType")
			.value("TEXT", framework::WebSocketExecutor::Frame::Type::text)
			.value("BINARY", framework::WebSocketExecutor::Frame::Type::binary)
			.export_values();

		frameClass
			.def
			(
				py::init
				(
					[](uint64_t pointer)
					{
						return framework::WebSocketExecutor::Frame(reinterpret_cast<void*>(pointer));
					}
				),
				"pointer"_a
			)
			.def
			(
				"get_payload",
				[](const framework::WebSocketExecutor::Frame& frame) -> std::string
				{
					std::span<char> payload = frame.getPayload<char>();

					return std::string(payload.data(), payload.size());
				}
			)
			.def
			(
				"get_payload",
				[](const framework::WebSocketExecutor::Frame& frame) -> std::vector<uint8_t>
				{
					std::span<uint8_t> payload = frame.getPayload<uint8_t>();

					return std::vector<uint8_t>(payload.begin(), payload.end());
				}
			)
			.def("get_type", &framework::WebSocketExecutor::Frame::getType)
			.def("__bool__", &framework::WebSocketExecutor::Frame::operator bool);

		py::class_<framework::WebSocketExecutor, framework::PyWebSocketExecutor>(m, "WebSocketExecutor")
			.def("on_receive", &framework::WebSocketExecutor::onReceive, "frame"_a);
	}
}
