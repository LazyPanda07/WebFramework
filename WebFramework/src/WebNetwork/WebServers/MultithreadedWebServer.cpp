#include "MultithreadedWebServer.h"

#include "WebNetwork/WebFrameworkHTTPNetwork.h"
#include "BaseIOSocketStream.h"
#include "Exceptions/NotImplementedException.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/CantFindFunctionException.h"
#include "Exceptions/MissingLoadTypeException.h"
#include "Exceptions/CantLoadSourceException.h"
#include "Exceptions/BadRequestException.h"
#include "Utility/RouteParameters.h"

#pragma warning(disable: 6387)

using namespace std;

namespace framework
{
	void MultithreadedWebServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		streams::IOSocketStream stream(new buffers::IOSocketBuffer(new WebFrameworkHTTPNetwork(clientSocket)));
		const string clientIp = getClientIpV4(addr);
		unordered_map<string, smartPointer<BaseExecutor>> statefulExecutors;
		HTTPResponse response;
		optional<function<void(HTTPRequest&&, HTTPResponse&)>> threadPoolFunction;

		while (true)
		{
			try
			{
				HTTPRequest request(sessionsManager, *this, *resources, *resources, databasesManager, addr);

				response.setDefault();

				stream >> request;

				executorsManager.service(move(request), response, statefulExecutors, threadPoolFunction);

				stream << response;
			}
			catch (const web::WebException&)
			{
				data.erase(clientIp);

				for (auto& i : statefulExecutors)
				{
					i.second->destroy();
				}

				break;
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
	}

	MultithreadedWebServer::MultithreadedWebServer(const vector<utility::JSONSettingsParser>& parsers, const filesystem::path& assets, const string& pathToTemplates, bool isCaching, const string& ip, const string& port, DWORD timeout, const vector<string>& pathToSources) :
		BaseTCPServer
		(
			port,
			ip,
			timeout,
			true,
			0,
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
		)
	{
		
	}
}
