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
		/// <summary>
		/// Tries to balance load of all servers
		/// </summary>
		class LoadBalancerServer : public web::BaseTCPServer
		{
		public:
			/// <summary>
			/// data for IOSocketStream connection
			/// </summary>
			struct connectionData
			{
				std::string ip;
				std::string port;
				unsigned int connections;

				/// <summary>
				/// Construct connectionData from LoadBalancerServer constructor or from disconnectUser method
				/// </summary>
				/// <param name="ip">server's address</param>
				/// <param name="port">server's port</param>
				/// <param name="connections">current connections to server</param>
				connectionData(const std::string& ip, const std::string& port, unsigned int connections = 0);

				/// <summary>
				/// Move constructor
				/// </summary>
				/// <param name="ip">server's address</param>
				/// <param name="port">server's port</param>
				/// <param name="connections">current connections to server</param>
				connectionData(std::string&& ip, std::string&& port, unsigned int connections) noexcept;

				/// <summary>
				/// Operators for PriorityQueue sorting
				/// </summary>
				/// <param name="other">another connectionData instance</param>
				/// <returns>current instance is smaller than other instance</returns>
				bool operator > (const connectionData& other) const;

				/// <summary>
				/// Operators for PriorityQueue find method
				/// </summary>
				/// <param name="other">another connectionData instance</param>
				/// <returns>current instance is equals than other instance</returns>
				bool operator == (const connectionData& other) const;
			};

		private:
			utility::PriorityQueue<connectionData, std::vector<connectionData>, std::greater<connectionData>> allServers;
			std::shared_mutex allServersMutex;

		private:
			/// <summary>
			/// Choose server with smallest connections to connect to
			/// </summary>
			/// <returns>data for connection</returns>
			connectionData chooseServer();

			/// <summary>
			/// Rearrange allServers after disconnection
			/// </summary>
			/// <param name="data">current server connection data with server</param>
			void disconnectUser(const connectionData& data);

		private:
			/// <summary>
			/// Called for every client
			/// </summary>
			/// <param name="clientSocket">client's socket</param>
			/// <param name="addr">client's address</param>
			void clientConnection(SOCKET clientSocket, sockaddr addr) override;

		public:
			/// <summary>
			/// Construct LoadBalancerServer
			/// </summary>
			/// <param name="port">listen server port</param>
			/// <param name="ip">server's address</param>
			/// <param name="timeout">time to wait for connection</param>
			/// <param name="allServers">list of all available servers</param>
			LoadBalancerServer(const std::string& port, const std::string& ip, DWORD timeout, const std::unordered_multimap<std::string, std::string>& allServers);

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
