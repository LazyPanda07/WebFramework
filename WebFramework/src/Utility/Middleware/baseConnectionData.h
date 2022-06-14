#pragma once

#include "headers.h"

namespace framework
{
	namespace utility
	{
		/// <summary>
		/// Contains all data for connection to server
		/// </summary>
		struct WEB_FRAMEWORK_API baseConnectionData
		{
		public:
			std::string ip;
			std::string port;

		public:
			/// <summary>
			/// Construct baseConnectionData from raw ip and port
			/// </summary>
			/// <param name="ip">server's address</param>
			/// <param name="port">server's port</param>
			baseConnectionData(const std::string& ip, const std::string& port);

			/// <summary>
			/// Construct baseConnectionData from raw ip and port
			/// </summary>
			/// <param name="ip">server's address</param>
			/// <param name="port">server's port</param>
			baseConnectionData(std::string&& ip, std::string&& port) noexcept;

			virtual ~baseConnectionData() = default;
		};
	}
}
