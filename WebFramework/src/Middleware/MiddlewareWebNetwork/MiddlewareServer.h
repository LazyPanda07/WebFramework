#pragma once

#include "BaseTCPServer.h"
#include "Middleware/BaseMiddlewareRouteController.h"

namespace framework
{
	namespace middleware
	{
		class MiddlewareServer : public web::BaseTCPServer
		{
		private:
			BaseMiddlewareRouteController& controller;

		private:
			void clientConnection(SOCKET clientSocket, sockaddr addr) override;

		public:
			MiddlewareServer(const std::string& port, const std::string& ip, DWORD timeout, const std::vector<utility::baseConnectionData>& servers, BaseMiddlewareRouteController& controller);

			~MiddlewareServer() = default;
		};
	}
}
