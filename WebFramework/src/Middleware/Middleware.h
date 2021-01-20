#pragma once

#include <vector>

#include "BaseMiddlewareRouteController.h"
#include "MiddlewareWebNetwork/MiddlewareServer.h"

namespace framework
{
	namespace middleware
	{
		/// <summary>
		/// <para>Main class of Middleware servers</para>
		/// <para>All initializations go through this class</para>
		/// </summary>
		class Middleware
		{
		private:
			MiddlewareServer middlewareServer;

		public:
			/// <summary>
			/// Construct Middleware
			/// </summary>
			/// <param name="ip">Middleware server ip</param>
			/// <param name="port">Middleware server port</param>
			/// <param name="timeout">time to wait for connections</param>
			/// <param name="controller">subclass of BaseMiddlewareRouteController that overrides getServerConnectionData method</param>
			Middleware(const std::string& ip, const std::string& port, DWORD timeout, BaseMiddlewareRouteController& controller);

			/// <summary>
			/// Start MiddlwareServer
			/// </summary>
			void startMiddleware();

			/// <summary>
			/// Stop MiddlwareServer incoming connections
			/// </summary>
			void stopMiddleware();

			/// <summary>
			/// Is server running
			/// </summary>
			/// <returns>is server running</returns>
			bool middlewareState() const;

			~Middleware() = default;
		};
	}
}
