#pragma once

#include "core.h"

#include "HTTPNetwork.h"

#include "Utility/Middleware/BaseConnectionData.h"

namespace framework
{
	class WebFrameworkHTTPNetwork : public web::HTTPNetwork
	{
	public:
		WebFrameworkHTTPNetwork(SOCKET clientSocket);

		WebFrameworkHTTPNetwork(std::string_view ip, std::string_view port, DWORD timeout = 30'000);

		WebFrameworkHTTPNetwork(const utility::BaseConnectionData& data);

		~WebFrameworkHTTPNetwork() = default;
	};
}
