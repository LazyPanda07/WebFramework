#include "MiddlewareServer.h"

#include "BaseIOSocketStream.h"
#include "WebNetwork/HTTPNetwork.h"

using namespace std;

namespace framework
{
	namespace middleware
	{
		void MiddlewareServer::clientConnection(SOCKET clientSocket, sockaddr addr)
		{
			streams::IOSocketStream clientStream(new buffers::IOSocketBuffer(new HTTPNetwork(clientSocket)));
			unique_ptr<streams::IOSocketStream> serverStream;

			while (true)
			{
				try
				{
					string request;
					string response;

					clientStream >> request;

					serverStream = make_unique<streams::IOSocketStream>(new buffers::IOSocketBuffer(new HTTPNetwork(controller.getServerConnectionData(web::HTTPParser(request)))));

					(*serverStream) << request;
					(*serverStream) >> response;

					clientStream << response;
				}
				catch (const web::WebException&)
				{
					break;
				}
			}

		}

		MiddlewareServer::MiddlewareServer(const string& ip, const string& port, DWORD timeout, BaseMiddlewareRouteController& controller) :
			BaseTCPServer(port, ip, timeout, false),
			controller(controller)
		{

		}
	}
}
