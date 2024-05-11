#include "WebFrameworkHTTPSNetwork.h"

using namespace std;

namespace framework
{
	WebFrameworkHTTPSNetwork::WebFrameworkHTTPSNetwork(SOCKET clientSocket, SSL* ssl, SSL_CTX* context) :
		HTTPSNetwork(clientSocket, ssl, context)
	{

	}

	WebFrameworkHTTPSNetwork::WebFrameworkHTTPSNetwork(string_view ip, string_view port, DWORD timeout) :
		HTTPSNetwork(ip, port, timeout)
	{

	}

	WebFrameworkHTTPSNetwork::WebFrameworkHTTPSNetwork(const utility::BaseConnectionData& data) :
		HTTPSNetwork(data.ip, data.port, data.timeout)
	{

	}
}
