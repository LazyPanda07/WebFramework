#include "MiddlewareServer.h"

#include "BaseIOSocketStream.h"
#include "WebNetwork/WebFrameworkHTTPNetwork.h"

using namespace std;

namespace framework
{
	namespace middleware
	{
		void MiddlewareServer::clientConnection(SOCKET clientSocket, sockaddr addr)
		{
			streams::IOSocketStream clientStream(make_unique<buffers::IOSocketBuffer>(make_unique<WebFrameworkHTTPNetwork>(clientSocket)));
			smartPointer<streams::IOSocketStream> serverStream;

			while (true)
			{
				try
				{
					string request;
					string response;

					clientStream >> request;

					serverStream = make_unique<streams::IOSocketStream>(make_unique<buffers::IOSocketBuffer>(make_unique<WebFrameworkHTTPNetwork>(controller.getServerConnectionData(web::HTTPParser(request)))));

					(*serverStream) << request;
					(*serverStream) >> response;

					clientStream << response;
				}
				catch (const web::exceptions::WebException&)
				{
					break;
				}
			}

		}

		MiddlewareServer::MiddlewareServer(const string& ip, const string& port, DWORD timeout, BaseMiddlewareRouteController& controller) :
			BaseTCPServer
			(
				port,
				ip,
				timeout,
				true,
				0,
				false
			),
			controller(controller)
		{

		}
	}
}
