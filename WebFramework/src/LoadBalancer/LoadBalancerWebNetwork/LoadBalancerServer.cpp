#include "LoadBalancerServer.h"

#include "WebNetwork/HTTPNetwork.h"
#include "WebNetwork/HTTPRequest.h"
#include "WebNetwork/HTTPResponse.h"
#include "BaseIOSocketStream.h"

using namespace std;

namespace framework
{
	namespace load_balancer
	{
		void LoadBalancerServer::clientConnection(SOCKET clientSocket, sockaddr addr)
		{
			streams::IOSocketStream stream(new buffers::IOSocketBuffer(new HTTPNetwork(clientSocket)));
			const string clientIp = getIpV4(addr);
			HTTPResponse response;
			
			while (true)
			{
				
			}
		}

		LoadBalancerServer::LoadBalancerServer(const string& port, const string& ip, DWORD timeout) :
			BaseTCPServer(port, ip, timeout, false)
		{

		}
	}
}
