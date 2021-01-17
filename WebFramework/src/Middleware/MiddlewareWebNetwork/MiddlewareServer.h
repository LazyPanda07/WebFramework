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
			/// <summary>
			/// Called for every client
			/// </summary>
			/// <param name="clientSocket">client's socket</param>
			/// <param name="addr">client's address</param>
			void clientConnection(SOCKET clientSocket, sockaddr addr) override;

		public:
			/// <summary>
			/// Construct MiddlewareServer
			/// </summary>
			/// <param name="port">server's port</param>
			/// <param name="ip">server's address</param>
			/// <param name="timeout">time to wait for connections</param>
			/// <param name="controller">subclass of BaseMiddlewareRouteController that overrides getServerConnectionData method</param>
			MiddlewareServer(const std::string& port, const std::string& ip, DWORD timeout, BaseMiddlewareRouteController& controller);

			~MiddlewareServer() = default;
		};
	}
}
