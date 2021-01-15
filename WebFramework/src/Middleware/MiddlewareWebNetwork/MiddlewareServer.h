#pragma once

#include "BaseTCPServer.h"

namespace framework
{
	namespace middleware
	{
		class MiddlewareServer : public web::BaseTCPServer
		{
		public:
			MiddlewareServer(const std::string& port, const std::string& ip, DWORD timeout);

			~MiddlewareServer() = default;
		};
	}
}
