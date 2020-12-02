#pragma once

#include "BaseTCPServer.h"
#include "WebFrameworkConstants.h"

namespace framework
{
	class WebServer : public web::BaseTCPServer
	{
	private:
		void clientConnection(SOCKET clientSocket, sockaddr addr) override;

	public:
		WebServer(const std::string_view& port = HTTPPort, DWORD timeout = 0);

		~WebServer() = default;
	};
}