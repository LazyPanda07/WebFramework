#include "ThreadPoolWebServer.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "WebNetwork/WebFrameworkHTTPNetwork.h"

using namespace std;

namespace framework
{
	ThreadPoolWebServer::IndividualData::IndividualData() :
		clientSocket(NULL),
		addr(),
		isBusy(false)
	{

	}

	ThreadPoolWebServer::IndividualData::IndividualData(SOCKET clientSocket, const sockaddr& addr) :
		clientSocket(clientSocket),
		addr(addr),
		clientIp(getClientIpV4(this->addr)),
		stream(new buffers::IOSocketBuffer(new framework::WebFrameworkHTTPNetwork(clientSocket))),
		isBusy(false)
	{

	}

	ThreadPoolWebServer::IndividualData::IndividualData(IndividualData&& other) noexcept :
		IndividualData()
	{
		clientSocket = other.clientSocket;
		addr = other.addr;
		clientIp = move(other.clientIp);
		statefulExecutors = move(other.statefulExecutors);
		stream = move(other.stream);
		isBusy = other.isBusy;
	}

	void ThreadPoolWebServer::taskImplementation(HTTPRequest&& request, IndividualData& client, function<void(HTTPRequest&&, HTTPResponse&)> executorMethod, vector<SOCKET>& disconnectedClients)
	{
		HTTPResponse response;

		try
		{
			executorMethod(move(request), response);

			client.stream << response;
		}
		catch (const web::WebException& e)
		{
			if (!e.getErrorCode())
			{
				unique_lock<mutex> lock(disconnectMutex);

				data.erase(client.clientIp);

				for (auto& i : client.statefulExecutors)
				{
					i.second->destroy();
				}

				disconnectedClients.push_back(client.clientSocket);
			}
		}
		catch (const exceptions::BadRequestException&)	// 400
		{
			resources->badRequestError(response);

			client.stream << response;
		}
		catch (const exceptions::FileDoesNotExistException&)	// 404
		{
			resources->notFoundError(response);

			client.stream << response;
		}
		catch (const exceptions::BaseExecutorException&)	//500
		{
			resources->internalServerError(response);

			client.stream << response;
		}
		catch (...)	//500
		{
			resources->internalServerError(response);

			client.stream << response;
		}

		client.isBusy = false;
	}

	void ThreadPoolWebServer::mainCycle(IndividualData& client, vector<SOCKET>& disconnectedClients)
	{
		HTTPRequest request(sessionsManager, *this, *resources, *resources, databasesManager, client.addr);
		HTTPResponse response;
		optional<function<void(HTTPRequest&&, HTTPResponse&)>> threadPoolFunction;

		try
		{
			client.stream >> request;

			executorsManager.service(move(request), response, client.statefulExecutors, threadPoolFunction);

			if (threadPoolFunction)
			{
				client.isBusy = true;

				function<void()> task = [this, &request, &client, threadPoolFunction, &disconnectedClients]()
				{
					taskImplementation(move(request), client, *threadPoolFunction, disconnectedClients);
				};

				threadPool.addTask(move(task));

				return;
			}

			client.stream << response;
		}
		catch (const web::WebException& e)
		{
			if (!e.getErrorCode())
			{
				unique_lock<mutex> lock(disconnectMutex);

				data.erase(client.clientIp);

				for (auto& i : client.statefulExecutors)
				{
					i.second->destroy();
				}

				disconnectedClients.push_back(client.clientSocket);
			}
		}
		catch (const exceptions::BadRequestException&)	// 400
		{
			resources->badRequestError(response);

			client.stream << response;
		}
		catch (const exceptions::FileDoesNotExistException&)	// 404
		{
			resources->notFoundError(response);

			client.stream << response;
		}
		catch (const exceptions::BaseExecutorException&)	//500
		{
			resources->internalServerError(response);

			client.stream << response;
		}
		catch (...)	//500
		{
			resources->internalServerError(response);

			client.stream << response;
		}
	}

	void ThreadPoolWebServer::receiveConnections()
	{
		vector<SOCKET> disconnectedClients;

		while (isRunning)
		{
			sockaddr addr;
			int addrlen = sizeof(addr);

			SOCKET clientSocket = accept(listenSocket, &addr, &addrlen);

			if (isRunning && clientSocket != INVALID_SOCKET)
			{
				setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&timeout), sizeof(timeout));

				ioctlsocket(clientSocket, FIONBIO, &blockingMode);

				data.insert(getClientIpV4(addr), clientSocket);

				this->clientConnection(clientSocket, addr);
			}

			for (auto& [clientSocket, client] : clients)
			{
				if (!client.isBusy)
				{
					this->mainCycle(client, disconnectedClients);
				}
			}

			for (const auto& i : disconnectedClients)
			{
				clients.erase(i);
			}

			disconnectedClients.clear();
		}
	}

	void ThreadPoolWebServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		if (clients.find(clientSocket) == clients.end())
		{
			clients.insert(make_pair(clientSocket, IndividualData(clientSocket, addr)));

			return;
		}
	}

	ThreadPoolWebServer::ThreadPoolWebServer(const vector<utility::JSONSettingsParser>& parsers, const filesystem::path& assets, const string& pathToTemplates, bool isCaching, const string& ip, const string& port, DWORD timeout, const vector<string>& pathToSources, uint32_t threadCount) :
		BaseTCPServer
		(
			port,
			ip,
			timeout,
			false,
			1,
			false
		),
		BaseWebServer
		(
			parsers,
			assets,
			pathToTemplates,
			isCaching,
			ip,
			port,
			timeout,
			pathToSources
		),
		threadPool(threadCount ? threadCount : thread::hardware_concurrency())
	{
		this->blockingMode = 1;
	}
}
