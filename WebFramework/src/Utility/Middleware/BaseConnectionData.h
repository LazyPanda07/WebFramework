#pragma once

#include "headers.h"

#include "Network.h"

namespace framework
{
	namespace utility
	{
		/// <summary>
		/// Contains all data for connection to server
		/// </summary>
		struct WEB_FRAMEWORK_API BaseConnectionData
		{
		public:
			std::string ip;
			std::string port;
			DWORD timeout;

		public:
			/// <summary>
			/// Construct BaseConnectionData from raw ip and port
			/// </summary>
			/// <param name="ip">server's address</param>
			/// <param name="port">server's port</param>
			BaseConnectionData(const std::string& ip, const std::string& port, DWORD timeout = 30'000);

			/// <summary>
			/// Construct BaseConnectionData from raw ip and port
			/// </summary>
			/// <param name="ip">server's address</param>
			/// <param name="port">server's port</param>
			BaseConnectionData(std::string&& ip, std::string&& port, DWORD timeout = 30'000) noexcept;

			virtual ~BaseConnectionData() = default;
		};
	}
}
