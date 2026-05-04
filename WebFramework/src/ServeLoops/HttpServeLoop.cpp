#include "ServeLoops/HttpServeLoop.h"

#include "Utility/LargeFileHandlers/MultithreadedHandler.h"

namespace framework::serve_loop
{
	void HttpServeLoop::init
	(
		const HttpServeRequest& serveRequest,
		SessionsManager& manager,
		BaseWebServer& server,
		ExecutorsManager& executorsManager,
		sockaddr address,
		const utility::AdditionalServerSettings& additionalSettings,
		std::queue<std::unique_ptr<event::ServeEvent>>& events
	)
	{
		web::http::HttpNetwork& network = stream.getNetwork<web::http::HttpNetwork>();

		network.setLargeBodyHandler<utility::MultithreadedHandler>(additionalSettings.largeBodyPacketSize, network, manager, server, resources, resources, address, stream, executorsManager, executors);
		network.setLargeBodySizeThreshold(additionalSettings.largeBodySizeThreshold);

		chain =
		{
			[this, &largeBodyHandler = stream.getNetwork<web::http::HttpNetwork>().getLargeBodyHandler()](ExecutorServer::ServiceState& state)
			{
				stream >> request;

				if (stream.eof()) // request may be empty
				{
					state = ExecutorServer::ServiceState::error;
				}
				else
				{
					state = largeBodyHandler.isRunning() ? ExecutorServer::ServiceState::skipResponse : ExecutorServer::ServiceState::success;
				}
			},
			std::bind(serveRequest, std::ref(request), std::ref(response), std::ref(executors), std::ref(events), std::placeholders::_1),
			[this](ExecutorServer::ServiceState& _)
			{
				stream << response;
			}
		};
	}

	bool HttpServeLoop::run()
	{
		const void* lastChainTask = &*chain.rbegin();
		const std::function<void(ExecutorServer::ServiceState&)>* task = &chain.front();

		response.setDefault();

		while (task)
		{
			switch (serveTask(stream, request, response, resources, *task))
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
	) :
		ServeLoop(std::move(stream), resources),
		serveTask(serveTask),
		request(manager, server, resources, resources, address, this->stream)
	{
		this->init(serveRequest, manager, server, executorsManager, address, additionalSettings, events);
	}
}
