#pragma once

#include "BaseNetwork.h"
#include "Utility/baseConnectionData.h"

namespace framework
{
	/// <summary>
	/// Network functions for HTTP
	/// </summary>
	class HTTPNetwork : public web::Network
	{
	public:
		/// <summary>
		/// Server side constructor
		/// </summary>
		/// <param name="clientSocket">socket from WSA accept function</param>
		HTTPNetwork(SOCKET clientSocket);

		/// <summary>
		/// Client side constructor
		/// </summary>
		/// <param name="ip">server address</param>
		/// <param name="port">server listen socket port</param>
		HTTPNetwork(const std::string& ip, const std::string& port);

		/// <summary>
		/// Client side constructor
		/// </summary>
		/// <param name="data">struct with ip and port</param>
		HTTPNetwork(const utility::baseConnectionData& data);

		/// <summary>
		/// Default send function
		/// </summary>
		/// <param name="data">that sends through HTTP</param>
		/// <returns>total send bytes</returns>
		int sendData(const std::vector<char>& data) override;

		/// <summary>
		/// Specific HTTP receive data function
		/// </summary>
		/// <param name="data">output data from HTTP</param>
		/// <returns>total receive bytes</returns>
		/// <exception cref="web::WebException"></exception>
		int receiveData(std::vector<char>& data) override;

		~HTTPNetwork() = default;
	};
}
