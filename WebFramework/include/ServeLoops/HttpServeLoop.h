#pragma once

#include "ServeLoop.h"

#include <array>

#include "Web/Servers/ExecutorServer.h"

namespace framework::serve_loop
{
	class HttpServeLoop : public ServeLoop 
	{
	public:
		using HttpServeTaskSignature = std::function<ExecutorServer::ServiceState(streams::IOSocketStream& stream, HttpRequestImplementation&, HttpResponseImplementation&, ResourceExecutor&, const std::function<void(ExecutorServer::ServiceState&)>&)>;
		using HttpServeRequest = std::function<void(HttpRequestImplementation&, HttpResponseImplementation&, ExecutorsManager::StatefulExecutors&, std::queue<std::unique_ptr<event::ServeEvent>>&, ExecutorServer::ServiceState&)>;

	private:
		HttpServeTaskSignature serveTask;
		std::array<std::function<void(ExecutorServer::ServiceState&)>, 3> chain;
		HttpRequestImplementation request;
		HttpResponseImplementation response;
		ExecutorsManager::StatefulExecutors executors;

	private:
		void init
		(
			const HttpServeRequest& serveRequest,
			SessionsManager& manager,
			BaseWebServer& server,
			ExecutorsManager& executorsManager,
			sockaddr address,
			const utility::AdditionalServerSettings& additionalSettings,
			std::queue<std::unique_ptr<event::ServeEvent>>& events
		);

	private:
		bool run() override;

	public:
		HttpServeLoop
		(
			streams::IOSocketStream&& stream,
			ResourceExecutor& resources,
			const HttpServeTaskSignature& serveTask,
			const HttpServeRequest& serveRequest,
			SessionsManager& manager,
			BaseWebServer& server,
			ExecutorsManager& executorsManager,
			sockaddr address,
			const utility::AdditionalServerSettings& additionalSettings,
			std::queue<std::unique_ptr<event::ServeEvent>>& events
		);

		~HttpServeLoop() = default;
	};
}
