#include "WebServer.h"

#include "HTTPNetwork.h"
#include "BaseIOSocketStream.h"
#include "Exceptions/NotImplementedException.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/CantFindFunctionException.h"
#include "Exceptions/MissingLoadTypeException.h"
#include "Exceptions/CantLoadSourceException.h"

#pragma warning(push)
#pragma warning(disable: 6387)

using namespace std;

namespace framework
{
	void WebServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		streams::IOSocketStream stream(new buffers::IOSocketBuffer(new HTTPNetwork(clientSocket)));
		const string clientIp = getIpV4(addr);
		unique_ptr<ResourceExecutor>& resources = executorsManager.getResourceExecutor();
		unordered_map<string, unique_ptr<BaseExecutor>> statefulExecutors;
		HTTPResponse response;

		while (true)
		{
			try
			{
				HTTPRequest request(sessionsManager, clientIp, *resources, *resources);

				response.setDefault();

				stream >> request;

				executorsManager.service(move(request), response, statefulExecutors);

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
			catch (const exceptions::NotImplementedException&)	// 400
			{
				resources->badRequestError(response);

				stream << response;
			}
			catch (const exceptions::FileDoesNotExistException&)	// 404
			{
				resources->notFoundError(response);

				stream << response;
			}
			catch (const out_of_range&)	// 404
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

	WebServer::WebServer(const utility::XMLSettingsParser& parser, const filesystem::path& assets, const string& pathToTemplates, bool isCaching, const string_view& port, DWORD timeout, const vector<string>& pathToSources) :
		BaseTCPServer(port, timeout, false)
	{
		unordered_map<string, unique_ptr<BaseExecutor>> routes;
		unordered_map<string, createBaseExecutorSubclassFunction> creator;
		unordered_map<string, utility::XMLSettingsParser::ExecutorSettings> settings = parser.getSettings();
		vector<HMODULE> sources = [&pathToSources]() -> vector<HMODULE>
		{
			vector<HMODULE> result;

			result.reserve(pathToSources.size());

			for (const auto& i : pathToSources)
			{
				if (!filesystem::exists(i))
				{
					if (i == ini::defaultLoadSourceValue)
					{
						result.push_back(nullptr);

						continue;
					}
					else
					{
						throw exceptions::FileDoesNotExistException(i);
					}
				}
				else
				{
					result.push_back(LoadLibraryA(i.data()));
				}

				if (!result.back())
				{
					throw exceptions::CantLoadSourceException(i);
				}
			}

			return result;
		}();

		routes.reserve(settings.size());
		creator.reserve(settings.size());

		for (const auto& [i, j] : settings)
		{
			createBaseExecutorSubclassFunction function = nullptr;

			for (const auto& source : sources)
			{
				function = reinterpret_cast<createBaseExecutorSubclassFunction>(GetProcAddress(source, ("create" + j.name + "Instance").data()));

				if (function)
				{
					break;
				}
			}

			if (!function)
			{
				throw exceptions::CantFindFunctionException("create" + j.name + "Instance");
			}

			switch (j.executorLoadType)
			{
			case utility::XMLSettingsParser::ExecutorSettings::loadType::initialization:
				routes[i] = unique_ptr<BaseExecutor>(function());

				if (routes[i]->getType() == BaseExecutor::executorType::stateful)
				{
					routes.erase(i);
				}
				else
				{
					routes[i]->init(j);
				}

				break;

			case utility::XMLSettingsParser::ExecutorSettings::loadType::dynamic:

				break;

			case utility::XMLSettingsParser::ExecutorSettings::loadType::none:
				throw exceptions::MissingLoadTypeException(j.name);

				break;
			}

			creator[j.name] = function;
		}

		executorsManager.init(assets, isCaching, pathToTemplates, move(routes), move(creator), move(settings));
	}
}

#pragma warning(pop)
