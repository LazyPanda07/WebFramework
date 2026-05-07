#pragma once

#include "BaseWebServer.h"
#include "ExecutorServer.h"

#include <chrono>
#include <queue>

#include "ThreadPool.h"
#include "Utility/LargeFileHandlers/BaseLargeBodyHandler.h"
#include "Events/ServeEvents/ServeEvent.h"
#include "ServeLoops/HttpServeLoop.h"

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
			std::unique_ptr<serve_loop::ServeLoop> loop;
			std::queue<std::unique_ptr<event::ServeEvent>> events;
			std::function<void()> cleanup;
			bool isBusy;
			bool webExceptionAcquired;

		public:
			Client
			(
				SSL* ssl, SOCKET clientSocket, sockaddr address,
				std::function<void()>&& cleanup,
				const serve_loop::HttpServeLoop::HttpServeTaskSignature& serveTask,
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
				BaseWebServer& server,
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
			const json::JsonParser& configuration,
			std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
			std::string_view ip,
			std::string_view port,
			DWORD timeout,
			const std::vector<std::string>& pathToSources,
			const utility::AdditionalServerSettings& additionalSettings,
			uint32_t numberOfThreads,
			std::shared_ptr<threading::ThreadPool> resourcesThreadPool,
			WebFramework& frameworkInstance
		);

		~ThreadPoolWebServer() = default;
	};
}
