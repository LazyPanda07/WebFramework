#pragma once

#include "BaseWebServer.h"
#include "ExecutorServer.h"

#include "ThreadPool.h"
#include "Utility/LargeFileHandlers/BaseLargeBodyHandler.h"

namespace framework
{
	class ThreadPoolWebServer :
		public virtual BaseWebServer,
		public ExecutorServer
	{
	private:
		class Client
		{
		private:
			streams::IOSocketStream stream;
			ExecutorsManager::StatefulExecutors executors;
			std::function<void()> cleanup;
			std::function<ExecutorServer::ServiceState(streams::IOSocketStream& stream, HTTPRequestImplementation&, HTTPResponseImplementation&, ResourceExecutor&, const std::function<void(ServiceState&)>&)> service;
			sockaddr address;
			bool isBusy;
			bool webExceptionAcquired;
			web::LargeBodyHandler* largeBodyHandler;

		public:
			Client
			(
				SSL* ssl, SSL_CTX* context, SOCKET clientSocket, sockaddr address,
				std::function<void()>&& cleanup,
				const std::function<ExecutorServer::ServiceState(streams::IOSocketStream&, HTTPRequestImplementation&, HTTPResponseImplementation&, ResourceExecutor&, const std::function<void(ServiceState&)>&)>& task,
				ThreadPoolWebServer& server,
				DWORD timeout
			);

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
			std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
			std::string_view ip,
			std::string_view port,
			DWORD timeout,
			const std::vector<std::string>& pathToSources,
			const utility::AdditionalServerSettings& additionalSettings,
			size_t numberOfThreads,
			std::shared_ptr<threading::ThreadPool> resourcesThreadPool
		);

		~ThreadPoolWebServer() = default;
	};
}
