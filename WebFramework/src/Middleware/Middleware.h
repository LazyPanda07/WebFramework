#pragma once

#include <filesystem>

#include "MiddlewareWebNetwork/MiddlewareServer.h"

namespace framework
{
	namespace middleware
	{
		class Middleware
		{
		private:
			std::unique_ptr<MiddlewareServer> middlewareServer;

		public:
			Middleware(const std::filesystem::path& configurationINIFile);

			~Middleware() = default;
		};
	}
}
