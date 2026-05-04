#include "Events/ServeEvents/UpgradeHttpConnect.h"

#include <WebSocket/WssNetwork.h>

namespace framework::event
{
	void UpgradeHttpConnect::operator ()(streams::IOSocketStream& stream, std::unique_ptr<serve_loop::ServeLoop>& loop) const
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
	}
}
