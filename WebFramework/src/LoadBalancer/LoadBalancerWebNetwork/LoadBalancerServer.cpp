#include "LoadBalancerServer.h"

#include "WebNetwork/HTTPNetwork.h"
#include "BaseIOSocketStream.h"

using namespace std;

namespace framework
{
	namespace load_balancer
	{
		LoadBalancerServer::connectionData LoadBalancerServer::chooseServer()
		{
			unique_lock<shared_mutex> lock(allServersMutex);
			connectionData data = allServers.top();

			allServers.pop();

			data.connections++;

			allServers.push(data);

			lock.unlock();

			return data;
		}

		void LoadBalancerServer::disconnectUser(const connectionData& data)
		{
			unique_lock<shared_mutex> lock(allServersMutex);

			auto valueFromContainer = allServers.find(data);
			connectionData valueToInsert(valueFromContainer->ip, valueFromContainer->port, valueFromContainer->connections - 1);

			allServers.erase(*valueFromContainer);

			allServers.push(move(valueToInsert));
		}

		LoadBalancerServer::connectionData::connectionData(const string& ip, const string& port, unsigned int connections) :
			ip(ip),
			port(port),
			connections(connections)
		{

		}

		LoadBalancerServer::connectionData::connectionData(string&& ip, string&& port, unsigned int connections) noexcept :
			ip(move(ip)),
			port(move(port)),
			connections(connections)
		{

		}

		void LoadBalancerServer::clientConnection(SOCKET clientSocket, sockaddr addr)
		{
			streams::IOSocketStream clientStream(new buffers::IOSocketBuffer(new HTTPNetwork(clientSocket)));
			const string clientIp = getIpV4(addr);
			connectionData data = this->chooseServer();
			streams::IOSocketStream serverStream(new buffers::IOSocketBuffer(new HTTPNetwork(data.ip, data.port)));
			
			while (true)
			{
				try
				{
					string request;
					string response;

					clientStream >> request;
					serverStream << request;

					serverStream >> response;
					clientStream << response;
				}
				catch (const web::WebException&)
				{
					this->disconnectUser(data);

					break;
				}
			}
		}

		LoadBalancerServer::LoadBalancerServer(const string& port, const string& ip, DWORD timeout, const unordered_map<string, string>& allServers) :
			BaseTCPServer(port, ip, timeout, false)
		{
			for (const auto& [key, value] : allServers)
			{
				this->allServers.emplace(key, value);
			}
		}
	}
}
