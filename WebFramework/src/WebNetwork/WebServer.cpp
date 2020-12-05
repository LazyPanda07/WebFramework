#include "WebServer.h"

#include "HTTPNetwork.h"
#include "BaseIOSocketStream.h"
#include "Exceptions/NotImplementedException.h"
#include "Exceptions/FileDoesNotExistException.h"

#pragma push
#pragma warning(disable: 6387)

using namespace std;

namespace framework
{
	void WebServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		streams::IOSocketStream stream(new buffers::IOSocketBuffer(new HTTPNetwork(clientSocket)));
		const string clientIp = getIpV4(addr);
		string response;

		while (true)
		{
			try
			{
				string HTTPRequest;

				response.clear();

				stream >> HTTPRequest;

				if (HTTPRequest.find("HTTP") != string::npos)
				{
					manager.service(web::HTTPParser(HTTPRequest), response);

					stream << response;
				}
				else
				{
					throw web::WebException();
				}
			}
			catch (const web::WebException&)
			{
				data.erase(clientIp);

				break;
			}
			catch (const exceptions::NotImplementedException&)
			{
				// 400
			}
			catch (const exceptions::BaseExecutorException&)
			{

			}
			catch (const out_of_range&)
			{
				// 404
			}
		}
	}

	WebServer::WebServer(const utility::XMLSettingsParser& parser, const string_view& port, DWORD timeout, const string& pathToExecutable) noexcept :
		BaseTCPServer(port, timeout, false)
	{
		unordered_map<string, unique_ptr<BaseExecutor>> routes;
		unordered_map<string, createBaseExecutorSubclassFunction> creator;
		unordered_map<string, utility::XMLSettingsParser::ExecutorSettings> settings = parser.getSettings();
		HMODULE executable = LoadLibraryA(pathToExecutable == ini::defaultLoadSourceValue ? nullptr : pathToExecutable.data());

		if (pathToExecutable != ini::defaultLoadSourceValue && !executable)
		{
			throw exceptions::FileDoesNotExistException();
		}

		routes.reserve(settings.size());
		creator.reserve(settings.size());

		for (const auto& [i, j] : settings)
		{
			createBaseExecutorSubclassFunction function = reinterpret_cast<createBaseExecutorSubclassFunction>(GetProcAddress(executable, ("create" + j.name + "Instance").data()));

			if (!function)
			{
				throw 15;
			}

			routes[i] = unique_ptr<BaseExecutor>(function());
			creator[j.name] = function;
		}

		manager.init(move(routes), move(creator), move(settings));
	}
}

#pragma pop
