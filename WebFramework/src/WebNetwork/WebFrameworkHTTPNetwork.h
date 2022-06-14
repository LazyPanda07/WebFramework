#pragma once

#include "headers.h"

#include "Utility/Middleware/baseConnectionData.h"

namespace framework
{
	class WebFrameworkHTTPNetwork : public web::HTTPNetwork
	{
	public:
		WebFrameworkHTTPNetwork(SOCKET clientSocket);

		WebFrameworkHTTPNetwork(const std::string& ip, const std::string& port);

		WebFrameworkHTTPNetwork(const utility::baseConnectionData& data);

		~WebFrameworkHTTPNetwork() = default;
	};
}
