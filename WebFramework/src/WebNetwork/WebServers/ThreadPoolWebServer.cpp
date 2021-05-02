#include "ThreadPoolWebServer.h"

#include "Exceptions/FileDoesNotExistException.h"

using namespace std;

namespace framework
{
	ThreadPoolWebServer::IndividualData::IndividualData(SOCKET clientSocket, const sockaddr& addr) :
		clientSocket(clientSocket),
		addr(addr),
		clientIp(getClientIpV4(this->addr))
	{

	}

	void ThreadPoolWebServer::mainCycle(IndividualData& client)
	{
		streams::IOSocketStream stream(client.clientSocket);
		HTTPRequest request(sessionsManager, *this, *resources, *resources, databasesManager, client.addr);
		HTTPResponse response;

		try
		{
			stream >> request;

			executorsManager.service(move(request), response, client.statefulExecutors);

			stream << response;
		}
		catch (const web::WebException& e)
		{
			cout << e.what() << " " << e.getErrorCode() << endl;

			/*if (!e.getErrorCode())
			{
				data.erase(client.clientIp);

				for (auto& i : client.statefulExecutors)
				{
					i.second->destroy();
				}
			}*/
		}
		catch (const exceptions::BadRequestException&)	// 400
		{
			resources->badRequestError(response);

			stream << response;
		}
		catch (const exceptions::FileDoesNotExistException&)	// 404
		{
			resources->notFoundError(response);

			stream << response;
		}
		catch (const exceptions::BaseExecutorException&)	//500
		{
			resources->internalServerError(response);

			stream << response;
		}
		catch (...)	//500
		{
			resources->internalServerError(response);

			stream << response;
		}
	}

	void ThreadPoolWebServer::receiveConnections()
	{
		BaseTCPServer::receiveConnections();

		for (auto& client : clients)
		{
			this->mainCycle(client);
		}
	}

	void ThreadPoolWebServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		if (sockets.find(clientSocket) == sockets.end())
		{
			sockets.insert(clientSocket);

			clients.emplace_back(clientSocket, addr);

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

	}
}
