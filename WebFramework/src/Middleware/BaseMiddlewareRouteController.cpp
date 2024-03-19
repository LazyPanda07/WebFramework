#include "BaseMiddlewareRouteController.h"

using namespace std;

namespace framework
{
	namespace middleware
	{
		BaseMiddlewareRouteController::BaseMiddlewareRouteController(initializer_list<utility::BaseConnectionData> servers) :
			servers(servers.begin(), servers.end())
		{

		}

		BaseMiddlewareRouteController::BaseMiddlewareRouteController(const vector<utility::BaseConnectionData>& servers) :
			servers(servers)
		{

		}
	}
}
