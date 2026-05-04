#pragma once

#include "ServeLoop.h"

#include <array>

#include "Web/Servers/ExecutorServer.h"

namespace framework::serve_loop
{
	class HttpServeLoop : public ServeLoop 
	{
	public:
		using HttpServiceSignature = std::function<ExecutorServer::ServiceState(streams::IOSocketStream& stream, HttpRequestImplementation&, HttpResponseImplementation&, ResourceExecutor&, const std::function<void(ExecutorServer::ServiceState&)>&)>;

	private:
		std::array<std::function<void(ExecutorServer::ServiceState&)>, 3> chain;
		HttpServiceSignature serviceFunction;
		HttpRequestImplementation request;

	protected:
		bool serve() override;

	public:
		HttpServeLoop
		(
			streams::IOSocketStream& stream, 
			ResourceExecutor& resources,
			const std::array<std::function<void(ExecutorServer::ServiceState&)>, 3>& chain, 
			const HttpServiceSignature& serviceFunction,
			SessionsManager& sessionsManager,
			BaseWebServer& server,
			sockaddr address
		);

		~HttpServeLoop() = default;
	};
}
