#include "ServeLoops/HttpServeLoop.h"

namespace framework::serve_loop
{
	bool HttpServeLoop::serve()
	{
		HttpResponseImplementation response;

		const void* lastChainTask = &*chain.rbegin();
		const std::function<void(ExecutorServer::ServiceState&)>* task = &chain.front();

		while (task)
		{
			switch (serviceFunction(stream, request, response, resources, *task))
			{
			case framework::ExecutorServer::ServiceState::success:
				task = task == lastChainTask ? nullptr : task + 1;

				break;

			case framework::ExecutorServer::ServiceState::skipResponse:
				task = nullptr;

				break;

			case framework::ExecutorServer::ServiceState::error:
				return true;
			}
		}

		return false;
	}

	HttpServeLoop::HttpServeLoop
	(
		streams::IOSocketStream& stream,
		ResourceExecutor& resources,
		const std::array<std::function<void(ExecutorServer::ServiceState&)>, 3>& chain, 
		const HttpServiceSignature& serviceFunction,
		SessionsManager& sessionsManager,
		BaseWebServer& server,
		sockaddr address
	) :
		ServeLoop(stream, resources),
		chain(chain),
		serviceFunction(serviceFunction),
		request(sessionsManager, server, resources, resources, address, stream)
	{

	}
}
