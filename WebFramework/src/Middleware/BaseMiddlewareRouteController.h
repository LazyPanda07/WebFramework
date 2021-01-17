#pragma once

#include <vector>

#include "HTTPParser.h"
#include "Utility/baseConnectionData.h"

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
			std::vector<utility::baseConnectionData> servers;

		public:
			/// <summary>
			/// Construct BaseMiddlewareRouteController through initializer_list
			/// </summary>
			/// <param name="servers">ips and ports for all servers</param>
			BaseMiddlewareRouteController(std::initializer_list<utility::baseConnectionData> servers);

			/// <summary>
			/// Copy construct
			/// </summary>
			/// <param name="servers">ips and ports for all servers</param>
			BaseMiddlewareRouteController(const std::vector<utility::baseConnectionData>& servers);

			/// <summary>
			/// Choose framework::utility::baseConnectionData
			/// </summary>
			/// <param name="httpMessage">client's request</param>
			/// <returns>value from servers member</returns>
			virtual const utility::baseConnectionData& getServerConnectionData(const web::HTTPParser& httpMessage) = 0;

			virtual ~BaseMiddlewareRouteController() = default;
		};
	}
}
