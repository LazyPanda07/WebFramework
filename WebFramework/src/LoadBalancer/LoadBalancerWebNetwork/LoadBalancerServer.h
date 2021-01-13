#pragma once

#include <unordered_map>
#include <queue>
#include <shared_mutex>

#include "BaseTCPServer.h"
#include "Utility/PriorityQueue.h"

namespace framework
{
	namespace load_balancer
	{
		class LoadBalancerServer : public web::BaseTCPServer
		{
		public:
			struct connectionData
			{
				std::string ip;
				std::string port;
				unsigned int connections;

				connectionData(const std::string& ip, const std::string& port, unsigned int connections = 0);

				connectionData(std::string&& ip, std::string&& port, unsigned int connections) noexcept;

				bool operator > (const connectionData& other) const;

				bool operator == (const connectionData& other) const;
			};

		private:
			utility::PriorityQueue<connectionData, std::vector<connectionData>, std::greater<connectionData>> allServers;
			std::shared_mutex allServersMutex;

		private:
			connectionData chooseServer();

			void disconnectUser(const connectionData& data);

		private:
			void clientConnection(SOCKET clientSocket, sockaddr addr) override;

		public:
			LoadBalancerServer(const std::string& port, const std::string& ip, DWORD timeout, const std::unordered_map<std::string, std::string>& allServers);

			~LoadBalancerServer() = default;
		};

		inline bool LoadBalancerServer::connectionData::operator > (const LoadBalancerServer::connectionData& other) const
		{
			return connections > other.connections;
		}

		inline bool LoadBalancerServer::connectionData::operator == (const LoadBalancerServer::connectionData& other) const
		{
			return ip == other.ip &&
				port == other.port;
		}
	}
}
