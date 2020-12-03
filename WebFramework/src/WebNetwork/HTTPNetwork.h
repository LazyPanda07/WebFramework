#pragma once

#include "BaseNetwork.h"

namespace framework
{
	class HTTPNetwork : public web::Network
	{
	public:
		HTTPNetwork(SOCKET clientSocket);

		int sendData(const std::vector<char>& data) override;

		int receiveData(std::vector<char>& data) override;

		~HTTPNetwork() = default;
	};
}
