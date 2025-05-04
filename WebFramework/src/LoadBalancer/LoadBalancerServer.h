#pragma once

#include "Framework/WebFrameworkPlatform.h"

#include "MultiLocalizationManager.h"

#include "WebNetwork/WebServers/BaseWebServer.h"
#include "Heuristics/BaseLoadBalancerHeuristic.h"
#include "Executors/ResourceExecutor.h"

#include "Utility/BaseConnectionData.h"

namespace framework
{
	namespace load_balancer
	{
		class LoadBalancerServer : public BaseWebServer
		{
		private:
			struct ServerData
			{
			public:
				utility::BaseConnectionData connectionData;
				std::unique_ptr<BaseLoadBalancerHeuristic> heuristic;

			public:
				ServerData(utility::BaseConnectionData&& connectionData, std::unique_ptr<BaseLoadBalancerHeuristic>&& heuristic) noexcept;

				ServerData(const ServerData&) = delete;

				ServerData(ServerData&&) noexcept = default;

				ServerData& operator = (const ServerData&) = delete;

				ServerData& operator = (ServerData&&) noexcept = default;

				~ServerData() = default;
			};

		private:
			std::vector<ServerData> allServers;
			std::shared_ptr<ResourceExecutor> resources;
			bool serversHTTPS;

		private:
			void clientConnection(const std::string& ip, SOCKET clientSocket, sockaddr addr, std::function<void()>& cleanup) override;

		public:
			LoadBalancerServer
			(
				std::string_view ip, std::string_view port, DWORD timeout, bool serversHTTPS,
				std::string_view heuristicName, const std::vector<HMODULE>& loadSources,
				const std::unordered_map<std::string, std::vector<int64_t>>& allServers,
				const json::JSONParser& configuration, const std::filesystem::path& assets, uint64_t cachingSize, const std::filesystem::path& pathToTemplates
			);

			~LoadBalancerServer() = default;
		};
	}
}
