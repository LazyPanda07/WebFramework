#include "MultithreadedHandler.h"

using namespace std;

namespace framework::utility
{
	MultithreadedHandler::MultithreadedHandler
	(
		web::Network& network, SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources,
		sockaddr clientAddr, streams::IOSocketStream& stream,
		ExecutorsManager& executorsManager, unordered_map<string, unique_ptr<BaseExecutor>>& statefulExecutors
	) :
		BaseLargeBodyHandler(network, session, serverReference, staticResources, dynamicResources, clientAddr, stream, executorsManager, statefulExecutors)
	{
		
	}

	MultithreadedHandler::WaitBehavior MultithreadedHandler::getWaitBehavior() const
	{
		return WaitBehavior::wait;
	}
}
