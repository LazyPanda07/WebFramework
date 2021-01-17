#pragma once

#include <vector>

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

			void startMiddleware();

			void stopMiddleware();

			bool middlewareState() const;

			~Middleware() = default;
		};
	}
}
