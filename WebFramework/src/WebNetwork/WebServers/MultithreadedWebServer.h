#pragma once

#include "core.h"

#include "BaseWebServer.h"

namespace framework
{
	class MultithreadedWebServer : public virtual BaseWebServer
	{
	private:
		void clientConnection(const std::string& ip, SOCKET clientSocket, const sockaddr& addr, std::function<void()>&& cleanup) override;

	public:
		MultithreadedWebServer(const json::JSONParser& configuration, const std::vector<utility::JSONSettingsParser>& parsers, const std::filesystem::path& assets, const std::string& pathToTemplates, uint64_t cachingSize, const std::string& ip, const std::string& port, DWORD timeout, const std::vector<std::string>& pathToSources);

		~MultithreadedWebServer() = default;
	};
}
