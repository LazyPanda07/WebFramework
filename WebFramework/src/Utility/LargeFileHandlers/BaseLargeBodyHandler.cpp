#include "BaseLargeBodyHandler.h"

#include <Log.h>

namespace framework::utility
{
	bool BaseLargeBodyHandler::handleChunk(std::string_view data)
	{
		requestWrapper->updateLargeData(data, this->isLast());

		try
		{
			std::invoke(method, executor, *requestWrapper, responseWrapper);
		}
		catch (const std::exception& e)
		{
			if (Log::isValid())
			{
				Log::error("Exception on handle chunk: {}", "LogBaseLargeBodyHandler", e.what());
			}

			return false;
		}

		return !static_cast<bool>(response);
	}

	void BaseLargeBodyHandler::onParseHeaders()
	{
		request = make_unique<HTTPRequestImplementation>(sessionsManager, serverReference, staticResources, dynamicResources, clientAddr, stream);
		response.setDefault();

		response.setIsValid(false);

		request->setParser(parser);

		requestWrapper = std::make_unique<HTTPRequestExecutors>(request.get());
		executor = executorsManager.getOrCreateExecutor(*requestWrapper, responseWrapper, executors);
		method = BaseExecutor::getMethod(parser.getMethod());
	}

	void BaseLargeBodyHandler::onFinishHandleChunks()
	{
		response.setIsValid(true);

		stream << response;
	}

	BaseLargeBodyHandler::BaseLargeBodyHandler
	(
		web::Network& network, SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources,
		sockaddr clientAddr, streams::IOSocketStream& stream,
		ExecutorsManager& executorsManager, ExecutorsManager::StatefulExecutors& executors
	) :
		LargeBodyHandler(network),
		sessionsManager(session),
		serverReference(serverReference),
		staticResources(staticResources),
		dynamicResources(dynamicResources),
		clientAddr(clientAddr),
		stream(stream),
		executorsManager(executorsManager),
		executors(executors),
		executor(nullptr),
		method(nullptr),
		responseWrapper(&response)
	{

	}
}
