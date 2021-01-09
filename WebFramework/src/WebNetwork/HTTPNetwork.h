#pragma once

#include "BaseNetwork.h"

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
