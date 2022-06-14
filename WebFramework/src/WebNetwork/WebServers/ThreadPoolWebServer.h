#pragma once

#include "headers.h"

#include "BaseWebServer.h"

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

			IndividualData(SOCKET clientSocket, const sockaddr& addr, SSL* ssl, SSL_CTX* context);

			IndividualData(const IndividualData&) = delete;

			IndividualData(IndividualData&& other) noexcept;

			IndividualData& operator = (const IndividualData&) = delete;

			IndividualData& operator = (IndividualData&&) noexcept = default;

			~IndividualData() = default;
		};
	private:
		threading::ThreadPool threadPool;
		std::unordered_map<SOCKET, IndividualData> clients;
		std::mutex disconnectMutex;

	private:
		void taskImplementation(HTTPRequest&& request, IndividualData& client, std::function<void(HTTPRequest&&, HTTPResponse&)> executorMethod, std::vector<SOCKET>& disconnectedClients, std::shared_ptr<ResourceExecutor>& resourceExecutor);

		void mainCycle(IndividualData& client, std::vector<SOCKET>& disconnectedClients, std::shared_ptr<ResourceExecutor>& resourceExecutor);

		void receiveConnections() override;

		void clientConnectionImplementation(SOCKET clientSocket, sockaddr addr, SSL* ssl, SSL_CTX* context);

		void clientConnection(SOCKET clientSocket, sockaddr addr) override;

	public:
		ThreadPoolWebServer(const json::JSONParser& configuration, const std::vector<utility::JSONSettingsParser>& parsers, const std::filesystem::path& assets, const std::string& pathToTemplates, uint64_t cachingSize, const std::string& ip, const std::string& port, DWORD timeout, const std::vector<std::string>& pathToSources, uint32_t threadCount);

		~ThreadPoolWebServer() = default;
	};
}
