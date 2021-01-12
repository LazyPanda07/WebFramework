#pragma once

#include "BaseTCPServer.h"

namespace framework
{
	namespace load_balancer
	{
		class LoadBalancerServer : public web::BaseTCPServer
		{
		private:
			void clientConnection(SOCKET clientSocket, sockaddr addr) override;

		public:
			LoadBalancerServer(const std::string& port, const std::string& ip, DWORD timeout);

			~LoadBalancerServer() = default;
		};
	}
}
