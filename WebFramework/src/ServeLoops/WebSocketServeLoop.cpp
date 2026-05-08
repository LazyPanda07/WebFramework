#include "ServeLoops/WebSocketServeLoop.h"

#include <bit>

#include <WebSocket/Frame.h>
#include <WebSocket/WsNetwork.h>

enum class Code : uint16_t
{
	normalClosure = 1000,
	goingAway,
	protocolError,
	unsupportedData,
	reserved,
	noStatusReceived,
	abnormalClosure,
	invalidFramePayloadData,
	policyViolation,
	messageTooBig,
	mandatoryExtension,
	internalError,
	serviceRestart,
	tryAgainLater,
	tLSHandshake = 1015
};

static constexpr std::array<std::pair<Code, std::string_view>, 15> defaultCodes =
{
	std::make_pair(Code::normalClosure, "The connection completed its purpose."),
	std::make_pair(Code::goingAway, "Server or browser is going away (tab closed, server restart)."),
	std::make_pair(Code::protocolError, "Endpoint received a malformed frame."),
	std::make_pair(Code::unsupportedData, "Endpoint received data type it cannot handle (e.g., text-only endpoint got binary)."),
	std::make_pair(Code::reserved, "Reserved for future use."),
	std::make_pair(Code::noStatusReceived, "Expected a status code but none was received."),
	std::make_pair(Code::abnormalClosure, "Connection was closed abnormally (no close frame sent)."),
	std::make_pair(Code::invalidFramePayloadData, "Data in a message is not consistent with its type."),
	std::make_pair(Code::policyViolation, "Endpoint received a message that violates its policy."),
	std::make_pair(Code::messageTooBig, "Message is too large to process."),
	std::make_pair(Code::mandatoryExtension, "Client expected the server to negotiate an extension."),
	std::make_pair(Code::internalError, "Server encountered an unexpected condition."),
	std::make_pair(Code::serviceRestart, "Server is restarting."),
	std::make_pair(Code::tryAgainLater, "Server is temporarily unable to handle the request."),
	std::make_pair(Code::tLSHandshake, "TLS handshake failure (not sent in frames).")
};

static constexpr bool operator ==(const std::pair<Code, std::string_view>& left, uint16_t right)
{
	return static_cast<uint16_t>(left.first) == right;
}

static web::web_socket::Frame createCloseFrame(uint16_t code, std::string_view payload);

namespace framework::serve_loop
{
	bool WebSocketServeLoop::run()
	{
		std::vector<web::web_socket::Frame> inputFrames;

		stream >> inputFrames;

		if (stream.eof())
		{
			return true;
		}

		for (const web::web_socket::Frame& inputFrame : inputFrames)
		{
			if (inputFrame.getFrameOpcode() == web::web_socket::Frame::OpcodeType::ping)
			{
				web::web_socket::Frame pong(true, web::web_socket::Frame::OpcodeType::pong, "");

				stream << pong;

				continue;
			}
			else if (inputFrame.getFrameOpcode() == web::web_socket::Frame::OpcodeType::pong)
			{
				continue;
			}

			if (std::optional<web::web_socket::Frame> outputFrame = executor->onReceive(inputFrame))
			{
				if (outputFrame->getFrameOpcode() == web::web_socket::Frame::OpcodeType::close)
				{
					const std::vector<uint8_t>& payload = outputFrame->getPayload();
					
					if (payload.empty())
					{
						web::web_socket::Frame temp(true, web::web_socket::Frame::OpcodeType::close, "");

						stream << temp;
					}
					else if (payload.size() == sizeof(uint16_t))
					{
						const uint16_t* code = reinterpret_cast<const uint16_t*>(payload.data());

						if (auto it = std::find(defaultCodes.begin(), defaultCodes.end(), *code); it != defaultCodes.end())
						{
							stream << createCloseFrame(*code, it->second);
						}
					}
					else
					{
						const uint16_t* code = reinterpret_cast<const uint16_t*>(payload.data());
						
						stream << createCloseFrame(*code, std::string_view(reinterpret_cast<const char*>(payload.data()), payload.size()));
					}

					return true;
				}

				stream << *outputFrame;
			}

			if (inputFrame.getFrameOpcode() == web::web_socket::Frame::OpcodeType::close)
			{
				return true;
			}
		}

		return false;
	}

	WebSocketServeLoop::WebSocketServeLoop(ServeLoop& serveLoop, std::unique_ptr<web_socket::WebSocketExecutor>&& executor) :
		ServeLoop(std::move(serveLoop.getStream()), serveLoop.getResources()),
		executor(std::move(executor))
	{

	}
}

web::web_socket::Frame createCloseFrame(uint16_t code, std::string_view payload)
{
	uint16_t newCode = std::byteswap(code);
	std::string newPayload(sizeof(code) + payload.size(), '\0');

	std::memcpy(newPayload.data(), &newCode, sizeof(newCode));
	std::memcpy(newPayload.data() + sizeof(newCode), payload.data(), payload.size());

	return web::web_socket::Frame(true, web::web_socket::Frame::OpcodeType::close, newPayload);
}
