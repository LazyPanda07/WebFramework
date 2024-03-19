#pragma once

#include "headers.h"

#include "Utility/Middleware/BaseConnectionData.h"

namespace framework
{
	class WebFrameworkHTTPNetwork : public web::HTTPNetwork
	{
	public:
		WebFrameworkHTTPNetwork(SOCKET clientSocket);

		WebFrameworkHTTPNetwork(const std::string& ip, const std::string& port);

		WebFrameworkHTTPNetwork(const utility::BaseConnectionData& data);

		~WebFrameworkHTTPNetwork() = default;
	};
}
