#pragma once

#include "BaseTCPServer.h"
#include "WebFrameworkConstants.h"
#include "Managers/ExecutorsManager.h"

namespace framework
{
	class WebServer : public web::BaseTCPServer
	{
	private:
		ExecutorsManager manager;

	private:
		void clientConnection(SOCKET clientSocket, sockaddr addr) override;

	public:
		WebServer(ExecutorsManager&& manager, const std::string_view& port = HTTPPort, DWORD timeout = 0);

		~WebServer() = default;
	};
}