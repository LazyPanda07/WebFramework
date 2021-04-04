#include "WebFrameworkHTTPNetwork.h"

using namespace std;

namespace framework
{
	WebFrameworkHTTPNetwork::WebFrameworkHTTPNetwork(SOCKET clientSocket) :
		HTTPNetwork(clientSocket)
	{

	}

	WebFrameworkHTTPNetwork::WebFrameworkHTTPNetwork(const string& ip, const string& port) :
		HTTPNetwork(ip, port)
	{

	}

	WebFrameworkHTTPNetwork::WebFrameworkHTTPNetwork(const utility::baseConnectionData& data) :
		WebFrameworkHTTPNetwork(data.ip, data.port)
	{
	
	}
}