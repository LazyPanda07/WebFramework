#include "Events/ServeEvents/UpgradeHttpConnect.h"

#include <WebSocket/WssNetwork.h>

#include "ServeLoops/WebSocketServeLoop.h"

namespace framework::event
{
	UpgradeHttpConnect::UpgradeHttpConnect(std::unique_ptr<web_socket::WebSocketExecutor>&& executor) :
		executor(std::move(executor))
	{

	}

	void UpgradeHttpConnect::operator ()(streams::IOSocketStream& stream, std::unique_ptr<serve_loop::ServeLoop>& loop)
	{
		web::http::HttpNetwork& network = stream.getNetwork<web::http::HttpNetwork>();

		if (web::http::HttpsNetwork* httpsNetwork = dynamic_cast<web::http::HttpsNetwork*>(&network))
		{
			stream = streams::IOSocketStream::createStream<web::web_socket::WssNetwork>(std::move(*httpsNetwork), false);
		}
		else
		{
			stream = streams::IOSocketStream::createStream<web::web_socket::WsNetwork>(std::move(network), false);
		}

		loop = std::make_unique<serve_loop::WebSocketServeLoop>(*loop, std::move(executor));
	}
}
