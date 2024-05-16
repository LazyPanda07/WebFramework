#pragma once

#include "headers.h"

#include "HTTPParser.h"

#include "Utility/Middleware/BaseConnectionData.h"

namespace framework
{
	namespace middleware
	{
		/// <summary>
		/// Specify server to connect with getServerConnectionData method
		/// </summary>
		class WEB_FRAMEWORK_API BaseMiddlewareRouteController
		{
		protected:
			std::vector<utility::BaseConnectionData> servers;

		public:
			/// <summary>
			/// Construct BaseMiddlewareRouteController through initializer_list
			/// </summary>
			/// <param name="servers">ips and ports for all servers</param>
			BaseMiddlewareRouteController(std::initializer_list<utility::BaseConnectionData> servers);

			/// <summary>
			/// Copy construct
			/// </summary>
			/// <param name="servers">ips and ports for all servers</param>
			BaseMiddlewareRouteController(const std::vector<utility::BaseConnectionData>& servers);

			/// <summary>
			/// Choose framework::utility::BaseConnectionData
			/// </summary>
			/// <param name="httpMessage">client's request</param>
			/// <returns>value from servers member</returns>
			virtual const utility::BaseConnectionData& getServerConnectionData(const web::HTTPParser& httpMessage) = 0;

			virtual ~BaseMiddlewareRouteController() = default;
		};
	}
}
