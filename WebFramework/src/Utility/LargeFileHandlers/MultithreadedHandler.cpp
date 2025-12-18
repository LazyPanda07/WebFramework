#include "MultithreadedHandler.h"

namespace framework::utility
{
	MultithreadedHandler::MultithreadedHandler
	(
		web::Network& network, SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources,
		sockaddr clientAddr, streams::IOSocketStream& stream,
		ExecutorsManager& executorsManager, ExecutorsManager::StatefulExecutors& executors
	) :
		BaseLargeBodyHandler(network, session, serverReference, staticResources, dynamicResources, clientAddr, stream, executorsManager, executors)
	{
		
	}

	MultithreadedHandler::WaitBehavior MultithreadedHandler::getWaitBehavior() const
	{
		return WaitBehavior::wait;
	}
}
