#pragma once

#include <vector>

#include "Utility/baseConnectionData.h"
#include "BaseMiddlewareRouteController.h"
#include "MiddlewareWebNetwork/MiddlewareServer.h"

namespace framework
{
	namespace middleware
	{
		class Middleware
		{
		private:
			MiddlewareServer middlewareServer;

		public:
			Middleware(const std::string& ip, const std::string& port, DWORD timeout, const std::vector<utility::baseConnectionData>& servers, BaseMiddlewareRouteController& controller);

			~Middleware() = default;
		};
	}
}
