#pragma once

#include <filesystem>

#include "WebNetwork/WebServer.h"

namespace framework
{
	class WebFramework
	{
	private:
		std::unique_ptr<WebServer> server;

	public:
		WebFramework(const std::filesystem::path& configurationINIFile);

		void startServer();

		void stopServer();

		std::vector<std::string> getClientsIp() const;

		void disconnectClient(const std::string& ip) const;

		bool getServerState() const;

		~WebFramework() = default;
	};
}
