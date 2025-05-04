#pragma once

#include "Framework/WebFrameworkPlatform.h"

#include "Network.h"

namespace framework
{
	namespace utility
	{
		/// <summary>
		/// Contains all data for connection to server
		/// </summary>
		struct BaseConnectionData
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
			BaseConnectionData(std::string_view ip, std::string_view port, DWORD timeout = 30'000);

			virtual ~BaseConnectionData() = default;
		};
	}
}
