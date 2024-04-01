#pragma once

#include "headers.h"

#include "Utility/Middleware/BaseConnectionData.h"

namespace framework
{
	class WebFrameworkHTTPNetwork : public web::HTTPNetwork
	{
	public:
		WebFrameworkHTTPNetwork(SOCKET clientSocket);

		WebFrameworkHTTPNetwork(std::string_view ip, std::string_view port);

		WebFrameworkHTTPNetwork(const utility::BaseConnectionData& data);

		~WebFrameworkHTTPNetwork() = default;
	};
}
