#include "BaseLargeBodyHandler.h"

using namespace std;

namespace framework::utility
{
	bool BaseLargeBodyHandler::handleChunk(string_view data, size_t bodySize)
	{
		request->updateLargeData(data, bodySize);

		try
		{
			invoke(method, executor, *request, response);
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
		request = make_unique<HTTPRequest>(sessionsManager, serverReference, staticResources, dynamicResources, database, clientAddr, stream);
		response.setDefault();

		response.setIsValid(false);

		const_cast<web::HTTPParser&>(request->getParser()) = parser;

		executor = executorsManager.getOrCreateExecutor(*request, response, *statefulExecutors);
		method = BaseExecutor::methods.at(parser.getMethod());
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
		ExecutorsManager& executorsManager
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
		statefulExecutors(nullptr),
		executor(nullptr),
		method(nullptr)
	{

	}

	void BaseLargeBodyHandler::setStatefulExecutors(std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& statefulExecutors)
	{
		this->statefulExecutors = &statefulExecutors;
	}
}
