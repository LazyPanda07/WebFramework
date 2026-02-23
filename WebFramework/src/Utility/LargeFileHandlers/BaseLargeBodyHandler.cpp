#include "BaseLargeBodyHandler.h"

#include <Log.h>

namespace framework::utility
{
	bool BaseLargeBodyHandler::handleChunk(std::string_view data)
	{
		request->updateLargeData(data.data(), data.size(), this->isLast());

		try
		{
			std::invoke(method, executor, *request, response);
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

		executor = executorsManager.getOrCreateExecutor(*request, response, executors);
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
		method(nullptr)
	{

	}
}
