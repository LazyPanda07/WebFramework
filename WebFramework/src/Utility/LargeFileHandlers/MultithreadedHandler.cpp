#include "MultithreadedHandler.h"

namespace framework::utility
{
	MultithreadedHandler::MultithreadedHandler(web::Network& network) :
		BaseLargeBodyHandler(network)
	{

	}

	MultithreadedHandler::WaitBehavior MultithreadedHandler::getWaitBehavior() const
	{
		return WaitBehavior::wait;
	}
}
