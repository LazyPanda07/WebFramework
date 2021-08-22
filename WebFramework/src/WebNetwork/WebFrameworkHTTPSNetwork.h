#pragma once

#include "headers.h"

#include "Utility/Middleware/baseConnectionData.h"

namespace framework
{
	/// @brief Network functions for HTTPSs
	class WebFrameworkHTTPSNetwork : public web::HTTPSNetwork
	{
	public:
		/// @brief Server side constructor
		/// @param ssl Result from SSL_new
		/// @param context Result from SSL_CTX_new
		/// @exception web::exceptions::SSLException
		WebFrameworkHTTPSNetwork(SOCKET clientSocket, SSL* ssl, SSL_CTX* context);

		/// @brief Client side constructor
		/// @param ip Server address
		/// @param port Server listen socket port
		/// @exception web::exceptions::SSLException
		WebFrameworkHTTPSNetwork(const std::string& ip, const std::string& port);

		/// @brief Client side constructor for middleware server connection
		/// @param data Struct with ip and port
		WebFrameworkHTTPSNetwork(const utility::baseConnectionData& data);

		~WebFrameworkHTTPSNetwork() = default;
	};
}
