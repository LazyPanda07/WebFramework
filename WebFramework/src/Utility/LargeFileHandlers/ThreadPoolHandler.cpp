#include "ThreadPoolHandler.h"

namespace framework::utility
{
	ThreadPoolHandler::ThreadPoolHandler
	(
		web::Network& network, SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources,
		sqlite::SQLiteManager& database, sockaddr clientAddr, streams::IOSocketStream& stream,
		ExecutorsManager& executorsManager
	) :
		BaseLargeBodyHandler(network, session, serverReference, staticResources, dynamicResources, database, clientAddr, stream, executorsManager)
	{

	}

	ThreadPoolHandler::WaitBehavior ThreadPoolHandler::getWaitBehavior() const
	{
		return WaitBehavior::exit;
	}
}
