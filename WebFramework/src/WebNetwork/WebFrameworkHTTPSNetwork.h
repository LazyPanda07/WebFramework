#pragma once

#include "headers.h"

#include "Utility/Middleware/baseConnectionData.h"

namespace framework
{
	class WebFrameworkHTTPSNetwork : public web::HTTPSNetwork
	{
	public:
		WebFrameworkHTTPSNetwork(SOCKET clientSocket, SSL* ssl, SSL_CTX* context);

		WebFrameworkHTTPSNetwork(const std::string& ip, const std::string& port);

		WebFrameworkHTTPSNetwork(const utility::baseConnectionData& data);

		~WebFrameworkHTTPSNetwork() = default;
	};
}
