#include "Middleware.h"

using namespace std;

namespace framework
{
	namespace middleware
	{
		Middleware::Middleware(const string& ip, const string& port, DWORD timeout, const vector<utility::baseConnectionData>& servers, BaseMiddlewareRouteController& controller) :
			middlewareServer(ip, port, timeout, servers, controller)
		{

		}
	}
}
