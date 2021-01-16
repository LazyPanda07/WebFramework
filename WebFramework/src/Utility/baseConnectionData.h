#pragma once

#include <string>

namespace framework
{
	namespace utility
	{
		struct baseConnectionData
		{
			std::string ip;
			std::string port;

			baseConnectionData(const std::string& ip, const std::string& port);

			baseConnectionData(std::string&& ip, std::string&& port) noexcept;

			virtual ~baseConnectionData() = default;
		};
	}
}
