#pragma once

#include "headers.h"

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

		public:
			/// <summary>
			/// Construct BaseConnectionData from raw ip and port
			/// </summary>
			/// <param name="ip">server's address</param>
			/// <param name="port">server's port</param>
			BaseConnectionData(const std::string& ip, const std::string& port);

			/// <summary>
			/// Construct BaseConnectionData from raw ip and port
			/// </summary>
			/// <param name="ip">server's address</param>
			/// <param name="port">server's port</param>
			BaseConnectionData(std::string&& ip, std::string&& port) noexcept;

			virtual ~BaseConnectionData() = default;
		};
	}
}
