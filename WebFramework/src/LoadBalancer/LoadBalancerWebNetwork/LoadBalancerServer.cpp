#include "LoadBalancerServer.h"

#include "WebNetwork/HTTPNetwork.h"
#include "BaseIOSocketStream.h"

using namespace std;

namespace framework
{
	namespace load_balancer
	{
		LoadBalancerServer::loadBalancerConnectionData LoadBalancerServer::chooseServer()
		{
			unique_lock<shared_mutex> lock(allServersMutex);
			loadBalancerConnectionData data = allServers.top();

			allServers.pop();

			data.connections++;

			allServers.push(data);

			lock.unlock();

			return data;
		}

		void LoadBalancerServer::disconnectUser(const loadBalancerConnectionData& data)
		{
			unique_lock<shared_mutex> lock(allServersMutex);

			auto valueFromContainer = allServers.find(data);
			loadBalancerConnectionData valueToInsert(valueFromContainer->ip, valueFromContainer->port, valueFromContainer->connections - 1);

			allServers.erase(*valueFromContainer);

			allServers.push(move(valueToInsert));
		}

		LoadBalancerServer::loadBalancerConnectionData::loadBalancerConnectionData(const string& ip, const string& port, unsigned int connections) :
			baseConnectionData(ip, port),
			connections(connections)
		{

		}

		LoadBalancerServer::loadBalancerConnectionData::loadBalancerConnectionData(string&& ip, string&& port, unsigned int connections) noexcept :
			baseConnectionData(move(ip),move(port)),
			connections(connections)
		{

		}

		void LoadBalancerServer::clientConnection(SOCKET clientSocket, sockaddr addr)
		{
			streams::IOSocketStream clientStream(new buffers::IOSocketBuffer(new HTTPNetwork(clientSocket)));
			const string clientIp = getIpV4(addr);
			loadBalancerConnectionData data = this->chooseServer();
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

		LoadBalancerServer::LoadBalancerServer(const string& port, const string& ip, DWORD timeout, const unordered_multimap<string, string>& allServers) :
			BaseTCPServer(port, ip, timeout, false)
		{
			for (const auto& [key, value] : allServers)
			{
				this->allServers.emplace(key, value);
			}
		}
	}
}
