#include "WebFrameworkHTTPNetwork.h"

using namespace std;

namespace framework
{
	WebFrameworkHTTPNetwork::WebFrameworkHTTPNetwork(SOCKET clientSocket) :
		HTTPNetwork(clientSocket)
	{

	}

	WebFrameworkHTTPNetwork::WebFrameworkHTTPNetwork(string_view ip, string_view port, DWORD timeout) :
		HTTPNetwork(ip, port, timeout)
	{

	}

	WebFrameworkHTTPNetwork::WebFrameworkHTTPNetwork(const utility::BaseConnectionData& data) :
		WebFrameworkHTTPNetwork(data.ip, data.port, data.timeout)
	{
	
	}
}
