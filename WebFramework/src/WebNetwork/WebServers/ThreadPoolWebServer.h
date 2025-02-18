#pragma once

#include "Import/WebFrameworkCore.h"

#include "ThreadPool.h"

#include "BaseWebServer.h"
#include "WebNetwork/Interfaces/IExecutorFunctionality.h"

namespace framework
{
	class ThreadPoolWebServer : 
		public virtual BaseWebServer,
		public interfaces::IExecutorFunctionality
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
			std::unique_ptr<HTTPRequest> largeRequest;
			BaseExecutor* largeExecutor = nullptr;
			void (BaseExecutor::* method)(HTTPRequest&, HTTPResponse&) = nullptr;

		public:
			Client(SSL* ssl, SSL_CTX* context, SOCKET clientSocket, sockaddr address, std::function<void()>&& cleanup);

			Client(const Client&) = delete;

			Client(Client&&) noexcept = delete;

			Client& operator = (const Client&) = delete;

			Client& operator = (Client&&) noexcept = delete;

			bool serve
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
		std::vector<Client*> clients;

	private:
		void serveClients();

	private:
		void clientConnection(const std::string& ip, SOCKET clientSocket, sockaddr address, std::function<void()>& cleanup) override;

		void onInvalidConnectionReceive() override;

	public:
		ThreadPoolWebServer
		(
			const json::JSONParser& configuration,
			const std::vector<utility::JSONSettingsParser>& parsers,
			const std::filesystem::path& assets,
			const std::filesystem::path& pathToTemplates,
			uint64_t cachingSize,
			std::string_view ip,
			std::string_view port,
			DWORD timeout,
			const std::vector<std::string>& pathToSources,
			uint32_t threadCount,
			const utility::AdditionalServerSettings& additionalSettings
		);

		~ThreadPoolWebServer() = default;
	};
}
