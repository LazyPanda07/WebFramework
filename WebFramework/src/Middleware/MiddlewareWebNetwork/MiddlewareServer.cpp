#include "MiddlewareServer.h"

using namespace std;

namespace framework
{
	namespace middleware
	{
		void MiddlewareServer::clientConnection(SOCKET clientSocket, sockaddr addr)
		{

		}

		MiddlewareServer::MiddlewareServer(const string& port, const string& ip, DWORD timeout, const vector<utility::baseConnectionData>& servers, BaseMiddlewareRouteController& controller) :
			BaseTCPServer(port, ip, timeout, false),
			controller(controller)
		{

		}
	}
}
