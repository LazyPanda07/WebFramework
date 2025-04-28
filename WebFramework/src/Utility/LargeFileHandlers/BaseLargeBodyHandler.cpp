#include "BaseLargeBodyHandler.h"

#include "Log.h"

using namespace std;

namespace framework::utility
{
	bool BaseLargeBodyHandler::handleChunk(string_view data, size_t bodySize)
	{
		request->updateLargeData(data.data(), data.size(), bodySize);

		try
		{
			// TODO: invoke

			// invoke(method, executor, *request, response);
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
		request = make_unique<HTTPRequestImplementation>(sessionsManager, serverReference, staticResources, dynamicResources, database, clientAddr, stream);
		response.setDefault();

		response.setIsValid(false);

		request->setParser(parser);

		// TOOD: getOrCreate
		// executor = executorsManager.getOrCreateExecutor(*request, response, statefulExecutors);
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
		sqlite::SQLiteManager& database, sockaddr clientAddr, streams::IOSocketStream& stream,
		ExecutorsManager& executorsManager, std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& statefulExecutors
	) :
		LargeBodyHandler(network),
		sessionsManager(session),
		serverReference(serverReference),
		staticResources(staticResources),
		dynamicResources(dynamicResources),
		database(database),
		clientAddr(clientAddr),
		stream(stream),
		executorsManager(executorsManager),
		statefulExecutors(statefulExecutors),
		executor(nullptr),
		method(nullptr)
	{

	}
}
