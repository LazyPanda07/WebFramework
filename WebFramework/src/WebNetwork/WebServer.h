#pragma once

#include "BaseTCPServer.h"
#include "WebFrameworkConstants.h"
#include "Managers/ExecutorsManager.h"
#include "Managers/SessionsManager.h"
#include "Utility/XMLSettingsParser.h"

namespace framework
{
	class WebServer : public web::BaseTCPServer
	{
	private:
		ExecutorsManager executorsManager;
		SessionsManager sessionsManager;

	private:
		void clientConnection(SOCKET clientSocket, sockaddr addr) override;

	public:
		WebServer(const utility::XMLSettingsParser& parser, const std::filesystem::path& assets, bool isCaching, const std::string_view& port = HTTPPort, DWORD timeout = 0, const std::vector<std::string>& pathToSources = {});

		~WebServer() = default;
	};
}