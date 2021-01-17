#pragma once

#include <string>

#ifdef MIDDLEWARE_DLL
#define WEB_FRAMEWORK_API __declspec(dllexport)
#else
#define WEB_FRAMEWORK_API
#endif // MIDDLEWARE_DLL

namespace framework
{
	namespace utility
	{
		struct WEB_FRAMEWORK_API baseConnectionData
		{
			std::string ip;
			std::string port;

			baseConnectionData(const std::string& ip, const std::string& port);

			baseConnectionData(std::string&& ip, std::string&& port) noexcept;

			virtual ~baseConnectionData() = default;
		};
	}
}
