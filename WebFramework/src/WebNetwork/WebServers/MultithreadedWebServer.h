#pragma once

#include "Framework/WebFrameworkPlatform.h"

#include "BaseWebServer.h"
#include "ExecutorServer.h"

namespace framework
{
	class MultithreadedWebServer : 
		public virtual BaseWebServer,
		public ExecutorServer
	{
	private:
		void clientConnection(const std::string& ip, SOCKET clientSocket, sockaddr addr, std::function<void()>& cleanup) override;

	public:
		MultithreadedWebServer
		(
			const json::JSONParser& configuration,
			std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
			std::string_view ip,
			std::string_view port,
			DWORD timeout,
			const std::vector<std::string>& pathToSources,
			const utility::AdditionalServerSettings& additionalSettings
		);

		~MultithreadedWebServer() = default;
	};
}
