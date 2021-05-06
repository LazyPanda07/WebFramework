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

	WebFrameworkHTTPSNetwork::WebFrameworkHTTPSNetwork(const string& ip, const string& port) :
		HTTPSNetwork
		(
			ip,
			port
		)
	{

	}

	WebFrameworkHTTPSNetwork::WebFrameworkHTTPSNetwork(const utility::baseConnectionData& data) :
		HTTPSNetwork
		(
			data.ip,
			data.port
		)
	{

	}
}
