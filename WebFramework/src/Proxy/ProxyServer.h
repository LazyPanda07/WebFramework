#pragma once

#include "Framework/WebFrameworkPlatform.h"

#include "Web/Servers/BaseWebServer.h"
#include "Utility/BaseConnectionData.h"

namespace framework
{
	namespace proxy
	{
		class ProxyServer : public BaseWebServer
		{
		private:
			struct ProxyData : public utility::BaseConnectionData
			{
			public:
				bool isHTTPS;

			public:
				ProxyData(std::string_view ip, std::string_view port, DWORD timeout, bool useHTTPS);

				~ProxyData() = default;
			};

		private:
			std::unordered_map<std::string, const ProxyData*> routes;
			std::vector<ProxyData> proxyData;

		private:
			void clientConnection(const std::string& ip, SOCKET clientSocket, sockaddr addr, std::function<void()>& cleanup) override;

		public:
			ProxyServer(std::string_view ip, std::string_view port, DWORD timeout, const json::utility::jsonObject& proxySettings);

			~ProxyServer() = default;
		};
	}
}
