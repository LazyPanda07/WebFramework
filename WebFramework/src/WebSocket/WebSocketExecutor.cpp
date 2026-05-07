#include "WebSocket/WebSocketExecutor.h"

#include <Log.h>

#include "Framework/WebFrameworkConstants.h"

namespace framework::web_socket
{
	void WebSocketExecutor::fixControlFrame(uint64_t& size, int32_t type)
	{
		if (type < static_cast<int32_t>(web::web_socket::Frame::OpcodeType::close) || size <= 125)
		{
			return;
		}

		if (Log::isValid())
		{
			std::string opcodeName;

			switch (static_cast<web::web_socket::Frame::OpcodeType>(type))
			{
			case web::web_socket::Frame::OpcodeType::close:
				opcodeName = "close";

				break;

			case web::web_socket::Frame::OpcodeType::ping:
				opcodeName = "ping";

				break;

			case web::web_socket::Frame::OpcodeType::pong:
				opcodeName = "ping";

				break;

			default:
				opcodeName = "Wrong type";

				break;
			}

			Log::warning<logging::message::wrongControlFrameSize, logging::category::webSocket>(size, opcodeName);
		}

		size = 125;
	}

	void WebSocketExecutor::defaultSendData(const uint8_t* data, uint64_t size, int32_t type, void* additionalData)
	{
		if (!data)
		{
			return;
		}

		WebSocketExecutor::fixControlFrame(size, type);

		*reinterpret_cast<std::optional<web::web_socket::Frame>*>(additionalData) = web::web_socket::Frame
		(
			true,
			static_cast<web::web_socket::Frame::OpcodeType>(type),
			std::string_view(reinterpret_cast<const char*>(data), size)
		);
	}
}
