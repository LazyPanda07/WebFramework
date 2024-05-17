#pragma once

#include "core.h"

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
			void clientConnection(const std::string& ip, SOCKET clientSocket, const sockaddr& addr, std::function<void()>&& cleanup) override;

		public:
			MiddlewareServer(const std::string& ip, const std::string& port, DWORD timeout, BaseMiddlewareRouteController& controller);

			~MiddlewareServer() = default;
		};
	}
}
