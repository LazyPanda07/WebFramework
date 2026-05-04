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
		using HttpServeRequest = std::function<void(interfaces::IHttpRequest&, interfaces::IHttpResponse&, ExecutorsManager::StatefulExecutors&, std::queue<std::unique_ptr<event::ServeEvent>>&)>;

	private:
		HttpServeTaskSignature serveTasks;
		std::array<std::function<void(ExecutorServer::ServiceState&)>, 3> chain;
		HttpRequestImplementation request;
		HttpResponseImplementation response;
		ExecutorsManager::StatefulExecutors executors;
		std::queue<std::unique_ptr<event::ServeEvent>> events;

	private:
		void init
		(
			const HttpServeRequest& serveRequest,
			SessionsManager& manager,
			BaseWebServer& server,
			ExecutorsManager& executorsManager,
			sockaddr address,
			const utility::AdditionalServerSettings& additionalSettings
		);

	protected:
		bool serve() override;

	public:
		HttpServeLoop
		(
			streams::IOSocketStream&& stream,
			ResourceExecutor& resources,
			const HttpServeTaskSignature& serveTasks,
			const HttpServeRequest& serveRequest,
			SessionsManager& manager,
			BaseWebServer& server,
			ExecutorsManager& executorsManager,
			sockaddr address,
			const utility::AdditionalServerSettings& additionalSettings
		);

		~HttpServeLoop() = default;
	};
}
