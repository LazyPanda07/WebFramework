#include "ServeLoops/WebSocketServeLoop.h"

#include <WebSocket/Frame.h>
#include <WebSocket/WsNetwork.h>

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

		// TODO: ping, pong, close from onReceive

		for (const web::web_socket::Frame& inputFrame : inputFrames)
		{
			std::optional<web::web_socket::Frame> outputFrame = executor->onReceive(inputFrame);

			if (inputFrame.getFrameOpcode() == web::web_socket::Frame::OpcodeType::close)
			{
				return true;
			}

			if (outputFrame)
			{
				stream << *outputFrame;
			}

			if (outputFrame && outputFrame->getFrameOpcode() == web::web_socket::Frame::OpcodeType::close)
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
