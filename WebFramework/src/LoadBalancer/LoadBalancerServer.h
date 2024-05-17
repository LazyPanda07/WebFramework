#pragma once

#include "core.h"

#include "BaseTCPServer.h"

#include "Utility/DataStructures/PriorityQueue.h"
#include "Utility/Middleware/BaseConnectionData.h"

namespace framework
{
	namespace load_balancer
	{
		class LoadBalancerServer : public web::BaseTCPServer
		{
		public:
			struct LoadBalancerConnectionData : public utility::BaseConnectionData
			{
			public:
				uint64_t connections;

			public:
				LoadBalancerConnectionData(std::string_view ip, std::string_view port, uint64_t connections = 0);

				bool operator > (const LoadBalancerConnectionData& other) const;

				bool operator == (const LoadBalancerConnectionData& other) const;

				~LoadBalancerConnectionData() = default;
			};

		private:
			utility::PriorityQueue<LoadBalancerConnectionData, std::vector<LoadBalancerConnectionData>, std::greater<LoadBalancerConnectionData>> allServers;
			std::shared_mutex allServersMutex;

		private:
			LoadBalancerConnectionData chooseServer();

			void disconnectUser(const LoadBalancerConnectionData& data);

		private:
			void clientConnection(const std::string& ip, SOCKET clientSocket, const sockaddr& addr, std::function<void()>&& cleanup) override;

		public:
			LoadBalancerServer(std::string_view ip, std::string_view port, DWORD timeout, const std::unordered_map<std::string, std::vector<std::string>>& allServers);

			~LoadBalancerServer() = default;
		};

		inline bool LoadBalancerServer::LoadBalancerConnectionData::operator > (const LoadBalancerConnectionData& other) const
		{
			return connections > other.connections;
		}

		inline bool LoadBalancerServer::LoadBalancerConnectionData::operator == (const LoadBalancerConnectionData& other) const
		{
			return ip == other.ip &&
				port == other.port;
		}
	}
}
