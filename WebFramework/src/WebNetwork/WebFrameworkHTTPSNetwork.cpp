#include "WebFrameworkHTTPSNetwork.h"

using namespace std;

namespace framework
{
	WebFrameworkHTTPSNetwork::WebFrameworkHTTPSNetwork(SOCKET clientSocket, SSL* ssl, SSL_CTX* context) :
		HTTPSNetwork
		(
			clientSocket,
			ssl,
			context
		)
	{

	}

	WebFrameworkHTTPSNetwork::WebFrameworkHTTPSNetwork(string_view ip, string_view port) :
		HTTPSNetwork(ip, port)
	{

	}

	WebFrameworkHTTPSNetwork::WebFrameworkHTTPSNetwork(const utility::BaseConnectionData& data) :
		HTTPSNetwork(data.ip, data.port)
	{

	}

	WebFrameworkHTTPSNetwork::~WebFrameworkHTTPSNetwork()
	{
		clientSocket = -1;
	}
}
