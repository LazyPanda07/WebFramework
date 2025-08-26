#include "BaseLargeBodyHandler.h"

#include "Log.h"

using namespace std;

namespace framework::utility
{
	bool BaseLargeBodyHandler::handleChunk(string_view data)
	{
		requestWrapper->updateLargeData(data, this->isLast());

		try
		{
			 invoke(method, executor, *requestWrapper, responseWrapper);
		}
		catch (const exception& e)
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

		requestWrapper = make_unique<HTTPRequestExecutors>(request.get());
		executor = executorsManager.getOrCreateExecutor(*requestWrapper, responseWrapper, statefulExecutors);
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
		ExecutorsManager& executorsManager, std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& statefulExecutors
	) :
		LargeBodyHandler(network),
		sessionsManager(session),
		serverReference(serverReference),
		staticResources(staticResources),
		dynamicResources(dynamicResources),
		clientAddr(clientAddr),
		stream(stream),
		executorsManager(executorsManager),
		statefulExecutors(statefulExecutors),
		executor(nullptr),
		method(nullptr),
		responseWrapper(&response)
	{

	}
}
