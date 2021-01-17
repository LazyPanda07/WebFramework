#pragma once

#include <vector>

#include "HTTPParser.h"
#include "Utility/baseConnectionData.h"

namespace framework
{
	namespace middleware
	{
		class WEB_FRAMEWORK_API BaseMiddlewareRouteController
		{
		protected:
			std::vector<utility::baseConnectionData> servers;

		public:
			BaseMiddlewareRouteController(std::initializer_list<utility::baseConnectionData> servers);

			BaseMiddlewareRouteController(const std::vector<utility::baseConnectionData>& servers);

			virtual const utility::baseConnectionData& getServerConnectionData(const web::HTTPParser& httpMessage) = 0;

			virtual ~BaseMiddlewareRouteController() = default;
		};
	}
}
