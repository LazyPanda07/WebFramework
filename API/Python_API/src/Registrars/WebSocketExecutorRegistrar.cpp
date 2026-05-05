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
				[](const framework::WebSocketExecutor::Frame& self) -> std::variant<std::string, py::bytes>
				{
					std::string_view result = self.getPayload();

					switch (self.getType())
					{
					case framework::WebSocketExecutor::Frame::Type::text:
						return std::string(result);

					case framework::WebSocketExecutor::Frame::Type::binary:
						return py::bytes(result);
					
					default:
						throw std::runtime_error(std::format("Wrong type: {}", static_cast<int>(self.getType())));
					}

					return result;
				}
			)
			.def
			(
				"get_payload_as_str",
				[](const framework::WebSocketExecutor::Frame& self) -> std::string
				{
					return std::string(self.getPayload());
				}
			)
			.def
			(
				"get_payload_as_bytes",
				[](const framework::WebSocketExecutor::Frame& self) -> py::bytes
				{
					std::string_view payload = self.getPayload();

					return py::bytes(payload);
				}
			)
			.def("get_type", &framework::WebSocketExecutor::Frame::getType)
			.def("__bool__", &framework::WebSocketExecutor::Frame::operator bool);

		py::class_<framework::WebSocketExecutor, framework::PyWebSocketExecutor>(m, "WebSocketExecutor")
			.def
			(
				"on_receive",
				[](framework::WebSocketExecutor& self, const framework::WebSocketExecutor::Frame& frame) -> std::variant<std::string, py::bytes>
				{
					std::variant<std::string, std::vector<uint8_t>> result = self.onReceive(frame);

					if (std::holds_alternative<std::string>(result))
					{
						return std::get<std::string>(result);
					}
					else if (std::holds_alternative<std::vector<uint8_t>>(result))
					{
						const std::vector<uint8_t>& temp = std::get<std::vector<uint8_t>>(result);

						return py::bytes(std::string_view(reinterpret_cast<const char*>(temp.data()), temp.size()));
					}

					throw std::runtime_error(std::format("Wrong result index: {}", result.index()));

					using namespace std::string_literals;

					return ""s;
				},
				"frame"_a
			);
	}
}
