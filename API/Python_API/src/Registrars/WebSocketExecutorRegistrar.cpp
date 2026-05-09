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
			.value("CONTINUATION", framework::WebSocketExecutor::Frame::Type::continuation)
			.value("TEXT", framework::WebSocketExecutor::Frame::Type::text)
			.value("BINARY", framework::WebSocketExecutor::Frame::Type::binary)
			.value("CLOSE", framework::WebSocketExecutor::Frame::Type::close)
			.value("PING", framework::WebSocketExecutor::Frame::Type::ping)
			.value("PONG", framework::WebSocketExecutor::Frame::Type::pong)
			.export_values();

		py::class_<framework::WebSocketExecutor::Frame::Close> frameClose(frameClass, "Close");

		py::enum_<framework::WebSocketExecutor::Frame::Close::Code>(frameClose, "Code")
			.value("NORMAL_CLOSURE", framework::WebSocketExecutor::Frame::Close::Code::normalClosure)
			.value("GOING_AWAY", framework::WebSocketExecutor::Frame::Close::Code::goingAway)
			.value("PROTOCOL_ERROR", framework::WebSocketExecutor::Frame::Close::Code::protocolError)
			.value("UNSUPPORTED_DATA", framework::WebSocketExecutor::Frame::Close::Code::unsupportedData)
			.value("INVALID_FRAME_PAYLOAD_DATA", framework::WebSocketExecutor::Frame::Close::Code::invalidFramePayloadData)
			.value("POLICY_VIOLATION", framework::WebSocketExecutor::Frame::Close::Code::policyViolation)
			.value("MESSAGE_TOO_BIG", framework::WebSocketExecutor::Frame::Close::Code::messageTooBig)
			.value("MANDATORY_EXTENSION", framework::WebSocketExecutor::Frame::Close::Code::mandatoryExtension)
			.value("INTERNAL_ERROR", framework::WebSocketExecutor::Frame::Close::Code::internalError)
			.value("SERVICE_RESTART", framework::WebSocketExecutor::Frame::Close::Code::serviceRestart)
			.value("TRY_AGAIN_LATER", framework::WebSocketExecutor::Frame::Close::Code::tryAgainLater)
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
					case framework::WebSocketExecutor::Frame::Type::continuation:
					case framework::WebSocketExecutor::Frame::Type::text:
					case framework::WebSocketExecutor::Frame::Type::close:
					case framework::WebSocketExecutor::Frame::Type::ping:
					case framework::WebSocketExecutor::Frame::Type::pong:
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

		frameClose
			.def(py::init())
			.def(py::init<framework::WebSocketExecutor::Frame::Close::Code>(), "code"_a)
			.def(py::init<framework::WebSocketExecutor::Frame::Close::Code, std::string_view>(), "code"_a, "message"_a)
			.def(py::init<uint16_t, std::string_view>(), "code"_a, "message"_a)
			.def
			(
				"make_data",
				[](framework::WebSocketExecutor::Frame::Close& self) -> py::bytes
				{
					uint64_t size;
					uint8_t* ptr = self.makeData(size);

					return py::bytes(std::string_view(reinterpret_cast<char*>(ptr), size));
				}
			);

		py::class_<framework::WebSocketExecutor, framework::PyWebSocketExecutor>(m, "WebSocketExecutor")
			.def(py::init())
			.def
			(
				"on_receive",
				[](framework::WebSocketExecutor& self, const framework::WebSocketExecutor::Frame& frame) -> std::variant<std::optional<std::variant<std::string, py::bytes>>, framework::WebSocketExecutor::Frame::Close>
				{
					std::optional<std::variant<std::string, py::bytes>> data;
					std::optional<framework::WebSocketExecutor::Frame::Close> close;

					if (std::optional<std::variant<std::string, std::vector<uint8_t>>> result = self.onReceive(frame, close))
					{
						if (std::holds_alternative<std::string>(*result))
						{
							data.emplace(std::get<std::string>(*result));
						}
						else if (std::holds_alternative<std::vector<uint8_t>>(*result))
						{
							const std::vector<uint8_t>& temp = std::get<std::vector<uint8_t>>(*result);

							data.emplace(py::bytes(std::string_view(reinterpret_cast<const char*>(temp.data()), temp.size())));
						}
						else
						{
							throw std::runtime_error(std::format("Wrong result index: {}", result->index()));
						}
					}
					else if (close)
					{
						return *close;
					}

					return data;
				},
				"frame"_a
			);
	}
}
