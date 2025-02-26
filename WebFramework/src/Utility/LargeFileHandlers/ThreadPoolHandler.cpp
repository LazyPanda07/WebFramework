#include "ThreadPoolHandler.h"

namespace framework::utility
{
	ThreadPoolHandler::ThreadPoolHandler(web::Network& network) :
		BaseLargeBodyHandler(network)
	{

	}

	ThreadPoolHandler::WaitBehavior ThreadPoolHandler::getWaitBehavior() const
	{
		return WaitBehavior::exit;
	}
}
