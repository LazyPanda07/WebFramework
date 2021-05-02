#pragma once

#include "BaseWebServer.h"
#include "ThreadPool.h"

namespace framework
{
	class ThreadPoolWebServer final : public virtual BaseWebServer
	{
	private:
		threading::ThreadPool threadPool;

	private:
		void receiveConnections() override;

		void clientConnection(SOCKET clientSocket, sockaddr addr) override;

	public:
		ThreadPoolWebServer(const std::vector<utility::JSONSettingsParser>& parsers, const std::filesystem::path& assets, const std::string& pathToTemplates, bool isCaching, const std::string& ip, const std::string& port, DWORD timeout, const std::vector<std::string>& pathToSources, uint32_t threadCount);

		~ThreadPoolWebServer() = default;
	};
}
