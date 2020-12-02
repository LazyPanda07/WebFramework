#include "WebServer.h"

#include "HTTPNetwork.h"
#include "BaseIOSocketStream.h"

using namespace std;

namespace framework
{
	void WebServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		streams::IOSocketStream stream(new buffers::IOSocketBuffer(new HTTPNetwork(clientSocket)));
		const string clientIp = getIpV4(addr);

		while (true)
		{
			try
			{
				string HTTPRequest;

				stream >> HTTPRequest;

				stream << HTTPRequest;
			}
			catch (const web::WebException&)
			{
				data.erase(clientIp);

				break;
			}
		}
	}

	WebServer::WebServer(const string_view& port, DWORD timeout) :
		BaseTCPServer(port, timeout, false)
	{

	}
}