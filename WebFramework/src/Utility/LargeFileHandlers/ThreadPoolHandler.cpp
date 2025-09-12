#include "ThreadPoolHandler.h"

using namespace std;

namespace framework::utility
{
	ThreadPoolHandler::ThreadPoolHandler
	(
		web::Network& network, SessionsManager& session, const web::BaseTCPServer& serverReference, interfaces::IStaticFile& staticResources, interfaces::IDynamicFile& dynamicResources,
		sockaddr clientAddr, streams::IOSocketStream& stream,
		ExecutorsManager& executorsManager, unordered_map<string, unique_ptr<BaseExecutor>>& statefulExecutors
	) :
		BaseLargeBodyHandler(network, session, serverReference, staticResources, dynamicResources, clientAddr, stream, executorsManager, statefulExecutors)
	{

	}

	ThreadPoolHandler::WaitBehavior ThreadPoolHandler::getWaitBehavior() const
	{
		return WaitBehavior::exit;
	}
}
