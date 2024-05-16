#pragma once

#include "headers.h"

#include "ThreadPool.h"

#include "BaseWebServer.h"

namespace framework
{
	class ThreadPoolWebServer : public virtual BaseWebServer
	{
	private:
		class Client
		{
		private:
			streams::IOSocketStream stream;
			std::unordered_map<std::string, std::unique_ptr<BaseExecutor>> statefulExecutors;
			std::function<void()> cleanup;
			sockaddr address;
			SSL* ssl;
			SSL_CTX* context;
			SOCKET clientSocket;
			bool isBusy;
			bool webExceptionAcquired;

		public:
			Client(SSL* ssl, SSL_CTX* context, SOCKET clientSocket, const sockaddr& address, std::function<void()>&& cleanup);

			Client(const Client&) = delete;

			Client(Client&&) noexcept = default;

			Client& operator = (const Client&) = delete;

			Client& operator = (Client&&) noexcept = default;

			bool clientServe
			(
				SessionsManager& sessionsManager,
				web::BaseTCPServer& server,
				interfaces::IStaticFile& staticResources,
				interfaces::IDynamicFile& dynamicResources,
				sqlite::SQLiteManager& databaseManager,
				ExecutorsManager& executorsManager,
				ResourceExecutor& resourceExecutor,
				threading::ThreadPool& threadPool
			);

			~Client();
		};

	private:
		threading::ThreadPool threadPool;
		std::vector<std::unique_ptr<Client>> clients;

	private:
		void serveClients();

	private:
		void clientConnection(const std::string& ip, SOCKET clientSocket, const sockaddr& address, std::function<void()>&& cleanup) override;

		void onInvalidConnectionReceive() override;

	public:
		ThreadPoolWebServer(const json::JSONParser& configuration, const std::vector<utility::JSONSettingsParser>& parsers, const std::filesystem::path& assets, const std::string& pathToTemplates, uint64_t cachingSize, const std::string& ip, const std::string& port, DWORD timeout, const std::vector<std::string>& pathToSources, uint32_t threadCount);

		~ThreadPoolWebServer() = default;
	};
}
