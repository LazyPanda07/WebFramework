#include "utilities.h"

#include "WebNetwork/WebFrameworkHTTPNetwork.h"
#include "WebNetwork/WebFrameworkHTTPSNetwork.h"

#include "settings.h"

namespace utility
{
	streams::IOSocketStream createSocketStream()
	{
		web::Network* network = nullptr;

		if (useHTTPS)
		{
			network = new framework::WebFrameworkHTTPSNetwork("127.0.0.1", "8080");
		}
		else
		{
			network = new framework::WebFrameworkHTTPNetwork("127.0.0.1", "8080");
		}

		return streams::IOSocketStream(std::unique_ptr<web::Network>(network));
	}
}
