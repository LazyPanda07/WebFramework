#include "BaseMiddlewareRouteController.h"

using namespace std;

namespace framework
{
	namespace middleware
	{
		BaseMiddlewareRouteController::BaseMiddlewareRouteController(initializer_list<utility::baseConnectionData> servers) :
			servers(servers.begin(), servers.end())
		{

		}

		BaseMiddlewareRouteController::BaseMiddlewareRouteController(const vector<utility::baseConnectionData>& servers) :
			servers(servers)
		{

		}
	}
}
