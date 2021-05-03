#include "LoadBalancerServer.h"

#include "WebNetwork/WebFrameworkHTTPNetwork.h"
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
			streams::IOSocketStream clientStream(new buffers::IOSocketBuffer(new WebFrameworkHTTPNetwork(clientSocket)));
			const string clientIp = getClientIpV4(addr);
			loadBalancerConnectionData data = this->chooseServer();
			streams::IOSocketStream serverStream(new buffers::IOSocketBuffer(new WebFrameworkHTTPNetwork(data.ip, data.port)));
			
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

		LoadBalancerServer::LoadBalancerServer(const string& ip, const string& port, DWORD timeout, const unordered_map<string, vector<string>>& allServers) :
			BaseTCPServer
			(
				port,
				ip,
				timeout,
				true,
				0,
				false
			)
		{
			for (const auto& [ip, ports] : allServers)
			{
				for (const auto& port : ports)
				{
					this->allServers.emplace(ip, port);
				}
			}
		}
	}
}
