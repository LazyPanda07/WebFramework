#include "ThreadPoolHandler.h"

namespace framework::utility
{
	ThreadPoolHandler::ThreadPoolHandler
	(
		web::Network& network, SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources,
		sockaddr clientAddr, streams::IOSocketStream& stream,
		ExecutorsManager& executorsManager, ExecutorsManager::StatefulExecutors& executors
	) :
		BaseLargeBodyHandler(network, session, serverReference, staticResources, dynamicResources, clientAddr, stream, executorsManager, executors)
	{

	}

	ThreadPoolHandler::WaitBehavior ThreadPoolHandler::getWaitBehavior() const
	{
		return WaitBehavior::exit;
	}
}
