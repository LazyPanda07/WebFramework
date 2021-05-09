#pragma once

#include "HTTPNetwork.h"
#include "Utility/Middleware/baseConnectionData.h"

namespace framework
{
	/// <summary>
	/// Network functions for HTTP
	/// </summary>
	class WebFrameworkHTTPNetwork : public web::HTTPNetwork
	{
	public:
		/// <summary>
		/// Server side constructor
		/// </summary>
		/// <param name="clientSocket">socket from WSA accept function</param>
		WebFrameworkHTTPNetwork(SOCKET clientSocket);

		/// <summary>
		/// Client side constructor
		/// </summary>
		/// <param name="ip">server address</param>
		/// <param name="port">server listen socket port</param>
		WebFrameworkHTTPNetwork(const std::string& ip, const std::string& port);

		/// <summary>
		/// Client side constructor for middleware server connection
		/// </summary>
		/// <param name="data">struct with ip and port</param>
		WebFrameworkHTTPNetwork(const utility::baseConnectionData& data);

		~WebFrameworkHTTPNetwork() = default;
	};
}
