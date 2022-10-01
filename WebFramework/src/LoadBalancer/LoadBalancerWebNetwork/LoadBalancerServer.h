#pragma once

#include "headers.h"

#include "Utility/DataStructures/PriorityQueue.h"
#include "Utility/Middleware/baseConnectionData.h"

namespace framework
{
	namespace load_balancer
	{
		class LoadBalancerServer : public web::BaseTCPServer
		{
		public:
			struct loadBalancerConnectionData : public utility::baseConnectionData
			{
			public:
				unsigned int connections;

			public:
				loadBalancerConnectionData(const std::string& ip, const std::string& port, unsigned int connections = 0);

				loadBalancerConnectionData(std::string&& ip, std::string&& port, unsigned int connections) noexcept;

				bool operator > (const loadBalancerConnectionData& other) const;

				bool operator == (const loadBalancerConnectionData& other) const;

				~loadBalancerConnectionData() = default;
			};

		private:
			utility::PriorityQueue<loadBalancerConnectionData, std::vector<loadBalancerConnectionData>, std::greater<loadBalancerConnectionData>> allServers;
			std::shared_mutex allServersMutex;

		private:
			loadBalancerConnectionData chooseServer();

			void disconnectUser(const loadBalancerConnectionData& data);

		private:
			void clientConnection(SOCKET clientSocket, sockaddr addr) override;

		public:
			LoadBalancerServer(const std::string& ip, const std::string& port, DWORD timeout, const std::unordered_map<std::string, std::vector<std::string>>& allServers);

			~LoadBalancerServer() = default;
		};

		inline bool LoadBalancerServer::loadBalancerConnectionData::operator > (const loadBalancerConnectionData& other) const
		{
			return connections > other.connections;
		}

		inline bool LoadBalancerServer::loadBalancerConnectionData::operator == (const loadBalancerConnectionData& other) const
		{
			return ip == other.ip &&
				port == other.port;
		}
	}
}
