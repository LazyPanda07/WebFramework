#pragma once

#include "core.h"

#include "WebNetwork/WebServers/BaseWebServer.h"
#include "MultiLocalizationManager.h"
#include "Heuristics/BaseLoadBalancerHeuristic.h"

#include "Utility/DataStructures/PriorityQueue.h"
#include "Utility/Middleware/BaseConnectionData.h"

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

		private:
			void clientConnection(const std::string& ip, SOCKET clientSocket, const sockaddr& addr, std::function<void()>&& cleanup) override;

		public:
			LoadBalancerServer
			(
				std::string_view ip, std::string_view port, DWORD timeout,
				std::string_view heuristicName, const std::vector<HMODULE>& loadSources,
				const std::unordered_map<std::string, std::vector<std::string>>& allServers
			);

			~LoadBalancerServer() = default;
		};
	}
}
