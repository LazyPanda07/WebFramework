#include "Middleware.h"

using namespace std;

namespace framework
{
	namespace middleware
	{
		Middleware::Middleware(const string& ip, const string& port, DWORD timeout, BaseMiddlewareRouteController& controller) :
			middlewareServer(port, ip, timeout, controller)
		{

		}

		void Middleware::startMiddleware()
		{
			middlewareServer.start();
		}

		void Middleware::stopMiddleware()
		{
			middlewareServer.stop();
		}

		bool Middleware::isMiddlewareRunning() const
		{
			return middlewareServer.isServerRunning();
		}
	}
}
