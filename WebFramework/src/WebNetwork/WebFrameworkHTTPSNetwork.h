#pragma once

#include "core.h"

#include "HTTPSNetwork.h"

#include "Utility/Middleware/BaseConnectionData.h"

namespace framework
{
	class WebFrameworkHTTPSNetwork : public web::HTTPSNetwork
	{
	public:
		WebFrameworkHTTPSNetwork(SOCKET clientSocket, SSL* ssl, SSL_CTX* context);

		WebFrameworkHTTPSNetwork(std::string_view ip, std::string_view port, DWORD timeout = 30'000);

		WebFrameworkHTTPSNetwork(const utility::BaseConnectionData& data);

		~WebFrameworkHTTPSNetwork() = default;
	};
}
