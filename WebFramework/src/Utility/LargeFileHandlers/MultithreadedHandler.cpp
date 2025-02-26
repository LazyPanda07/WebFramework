#include "MultithreadedHandler.h"

using namespace std;

//namespace framework::utility
//{
//	bool MultithreadedHandler::handleChunk(string_view data, size_t bodySize)
//	{
//		request->setLargeDataPart(data, bodySize);
//
//		try
//		{
//			invoke(method, executor, *request, response);
//		}
//		catch (const exception& e)
//		{
//			if (Log::isValid())
//			{
//				Log::error("Exception on handle chunk: {}", "LogMultithreadedHandler", e.what());
//			}
//
//			return false;
//		}
//
//		return !static_cast<bool>(response);
//	}
//
//	void MultithreadedHandler::onParseHeaders()
//	{
//		request = make_unique<HTTPRequest>(sessionsManager, serverReference, staticResources, dynamicResources, database, clientAddr, stream);
//		response.setDefault();
//		
//		response.setIsValid(false);
//
//		const_cast<web::HTTPParser&>(request->getParser()) = parser;
//
//		executor = executorsManager.getOrCreateExecutor(*request, response, statefulExecutors);
//		method = BaseExecutor::methods.at(parser.getMethod());
//	}
//
//	void MultithreadedHandler::onFinishHandleChunks()
//	{
//		response.setIsValid(true);
//
//		stream << response;
//	}
//
//	MultithreadedHandler::MultithreadedHandler
//	(
//		web::Network& network, SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources,
//		sqlite::SQLiteManager& database, sockaddr clientAddr, streams::IOSocketStream& stream,
//		ExecutorsManager& executorsManager, std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& statefulExecutors
//	) :
//		BaseLargeBodyHandler(network),
//		sessionsManager(session),
//		serverReference(serverReference),
//		staticResources(staticResources),
//		dynamicResources(dynamicResources),
//		database(database),
//		clientAddr(clientAddr),
//		stream(stream),
//		executorsManager(executorsManager),
//		statefulExecutors(statefulExecutors),
//		executor(nullptr),
//		method(nullptr)
//	{
//
//	}
//
//	MultithreadedHandler::WaitBehavior MultithreadedHandler::getWaitBehavior() const
//	{
//		return WaitBehavior::wait;
//	}
//}
