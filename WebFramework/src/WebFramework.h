#pragma once

#include "WebNetwork/WebServer.h"

namespace framework
{
	class WebFramework
	{
	private:
		std::unique_ptr<WebServer> server;

	public:
		WebFramework(const std::string& configurationINIFile);

		void startServer();

		void stopServer();

		std::vector<std::string> getClientsIp() const;

		void disconnectClient(const std::string& ip) const;

		~WebFramework() = default;
	};
}
