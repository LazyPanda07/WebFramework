#pragma once

#include "WebFrameworkCore.h"

#include "BaseWebServer.h"
#include "WebNetwork/Interfaces/IExecutorFunctionality.h"

namespace framework
{
	class MultithreadedWebServer : 
		public virtual BaseWebServer,
		public interfaces::IExecutorFunctionality
	{
	private:
		void clientConnection(const std::string& ip, SOCKET clientSocket, const sockaddr& addr, std::function<void()>&& cleanup) override;

	public:
		MultithreadedWebServer
		(
			const json::JSONParser& configuration,
			const std::vector<utility::JSONSettingsParser>& parsers,
			const std::filesystem::path& assets,
			const std::filesystem::path& pathToTemplates,
			uint64_t cachingSize,
			std::string_view ip,
			std::string_view port,
			DWORD timeout,
			const std::vector<std::string>& pathToSources
		);

		~MultithreadedWebServer() = default;
	};
}
