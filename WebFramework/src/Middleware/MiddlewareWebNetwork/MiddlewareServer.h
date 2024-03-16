#pragma once

#include "headers.h"

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
			void clientConnection(SOCKET clientSocket, const sockaddr& addr) override;

		public:
			MiddlewareServer(const std::string& ip, const std::string& port, DWORD timeout, BaseMiddlewareRouteController& controller);

			~MiddlewareServer() = default;
		};
	}
}
