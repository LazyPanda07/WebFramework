#pragma once

#include <unordered_map>

#include "BaseWebServer.h"
#include "ThreadPool.h"

namespace framework
{
	class ThreadPoolWebServer final : public virtual BaseWebServer
	{
	private:
		struct IndividualData
		{
			SOCKET clientSocket;
			sockaddr addr;
			std::string clientIp;
			std::unordered_map<std::string, smartPointer<BaseExecutor>> statefulExecutors;
			streams::IOSocketStream stream;
			bool isBusy;

			IndividualData();

			IndividualData(SOCKET clientSocket, const sockaddr& addr);

			IndividualData(const IndividualData&) = delete;

			IndividualData(IndividualData&& other) noexcept;

			IndividualData& operator = (const IndividualData&) = delete;

			IndividualData& operator = (IndividualData&&) noexcept = default;

			~IndividualData() = default;
		};

	private:
		threading::ThreadPool threadPool;
		std::unordered_map<SOCKET, IndividualData> clients;

	private:
		void mainCycle(IndividualData& client, std::vector<SOCKET>& disconnectedClients);

		void receiveConnections() override;

		void clientConnection(SOCKET clientSocket, sockaddr addr) override;

	public:
		ThreadPoolWebServer(const std::vector<utility::JSONSettingsParser>& parsers, const std::filesystem::path& assets, const std::string& pathToTemplates, bool isCaching, const std::string& ip, const std::string& port, DWORD timeout, const std::vector<std::string>& pathToSources, uint32_t threadCount);

		~ThreadPoolWebServer() = default;
	};
}
