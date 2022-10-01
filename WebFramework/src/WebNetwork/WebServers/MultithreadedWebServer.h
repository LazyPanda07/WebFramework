#pragma once

#include "headers.h"

#include "BaseWebServer.h"

namespace framework
{
	class MultithreadedWebServer final : public virtual BaseWebServer
	{
	private:
		void receiveConnections() override;

		void clientConnectionImplementation(SOCKET clientSocket, sockaddr addr, SSL* ssl, SSL_CTX* context);

		void clientConnection(SOCKET clientSocket, sockaddr addr) override;

	public:
		MultithreadedWebServer(const json::JSONParser& configuration, const std::vector<utility::JSONSettingsParser>& parsers, const std::filesystem::path& assets, const std::string& pathToTemplates, uint64_t cachingSize, const std::string& ip, const std::string& port, DWORD timeout, const std::vector<std::string>& pathToSources);

		~MultithreadedWebServer() = default;
	};
}
