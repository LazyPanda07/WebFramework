#include "BaseLargeBodyHandler.h"

using namespace std;

namespace framework::utility
{
	BaseLargeBodyHandler::BaseLargeBodyHandler(web::Network& network) :
		LargeBodyHandler(network)
	{

	}

	bool BaseLargeBodyHandler::handleChunk(string_view data, size_t& bodySize)
	{
		return true;
	}
}
