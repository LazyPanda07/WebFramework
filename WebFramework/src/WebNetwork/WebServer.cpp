#include "WebServer.h"

#include "HTTPNetwork.h"
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
	void WebServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		streams::IOSocketStream stream(new buffers::IOSocketBuffer(new HTTPNetwork(clientSocket)));
		const string clientIp = getIpV4(addr);
		smartPointer<ResourceExecutor>& resources = executorsManager.getResourceExecutor();
		unordered_map<string, smartPointer<BaseExecutor>> statefulExecutors;
		HTTPResponse response;

		while (true)
		{
			try
			{
				HTTPRequest request(sessionsManager, clientIp, *resources, *resources, databasesManager);

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

	WebServer::WebServer(const utility::JSONSettingsParser& parser, const filesystem::path& assets, const string& pathToTemplates, bool isCaching, const string& ip, const string& port, DWORD timeout, const vector<string>& pathToSources) :
		BaseTCPServer(port, ip, timeout, false)
	{
		unordered_map<string, smartPointer<BaseExecutor>> routes;
		unordered_map<string, createBaseExecutorSubclassFunction> creator;
		unordered_map<string, utility::JSONSettingsParser::ExecutorSettings> settings;// = parser.getSettings();
		const auto& tem = parser.getSettings();
		vector<utility::RouteParameters> routeParameters;
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

		for (const auto& i : tem)
		{
			utility::JSONSettingsParser::ExecutorSettings executorSettings;

			executorSettings.name = i.second.name;
			executorSettings.executorLoadType = i.second.executorLoadType;

			for (const auto& j : i.second.initParameters.data)
			{
				switch (j.second.index())
				{
				case json::utility::jNull:
					executorSettings.initParameters.data.insert(make_pair(j.first, get<json::utility::jNull>(j.second)));
					break;

				case json::utility::jString:
					executorSettings.initParameters.data.insert(make_pair(j.first, get<json::utility::jString>(j.second)));
					break;

				case json::utility::jChar:
					executorSettings.initParameters.data.insert(make_pair(j.first, get<json::utility::jChar>(j.second)));
					break;

				case json::utility::jUnsignedChar:
					executorSettings.initParameters.data.insert(make_pair(j.first, get<json::utility::jUnsignedChar>(j.second)));
					break;

				case json::utility::jBool:
					executorSettings.initParameters.data.insert(make_pair(j.first, get<json::utility::jBool>(j.second)));
					break;

				case json::utility::jInt64_t:
					executorSettings.initParameters.data.insert(make_pair(j.first, get<json::utility::jInt64_t>(j.second)));
					break;

				case json::utility::jUint64_t:
					executorSettings.initParameters.data.insert(make_pair(j.first, get<json::utility::jUint64_t>(j.second)));
					break;

				case json::utility::jDouble:
					executorSettings.initParameters.data.insert(make_pair(j.first, get<json::utility::jDouble>(j.second)));
					break;

				case json::utility::jNullArray:
					executorSettings.initParameters.data.insert(make_pair(j.first, get<json::utility::jNullArray>(j.second)));
					break;

				case json::utility::jStringArray:
					executorSettings.initParameters.data.insert(make_pair(j.first, get<json::utility::jStringArray>(j.second)));
					break;

				case json::utility::jCharArray:
					executorSettings.initParameters.data.insert(make_pair(j.first, get<json::utility::jCharArray>(j.second)));
					break;

				case json::utility::jUnsignedCharArray:
					executorSettings.initParameters.data.insert(make_pair(j.first, get<json::utility::jUnsignedCharArray>(j.second)));
					break;

				case json::utility::jBoolArray:
					executorSettings.initParameters.data.insert(make_pair(j.first, get<json::utility::jBoolArray>(j.second)));
					break;

				case json::utility::jInt64_tArray:
					executorSettings.initParameters.data.insert(make_pair(j.first, get<json::utility::jInt64_tArray>(j.second)));
					break;

				case json::utility::jUint64_tArray:
					executorSettings.initParameters.data.insert(make_pair(j.first, get<json::utility::jUint64_tArray>(j.second)));
					break;

				case json::utility::jDoubleArray:
					executorSettings.initParameters.data.insert(make_pair(j.first, get<json::utility::jDoubleArray>(j.second)));
					break;

				case json::utility::jJsonStruct:
					executorSettings.initParameters.data.insert(make_pair(j.first, get<json::utility::jJsonStruct>(j.second).get()));
					break;
				}
			}

			settings.insert(make_pair(i.first, move(executorSettings)));
		}

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
			case utility::JSONSettingsParser::ExecutorSettings::loadType::initialization:
				if (i.find('{') == string::npos)
				{
					auto [it, success] = routes.emplace(make_pair(i, smartPointer<BaseExecutor>(function())));

					if (success)
					{
						if (it->second->getType() == BaseExecutor::executorType::stateful)
						{
							routes.erase(i);
						}
						else
						{
							it->second->init(j);
						}
					}
				}
				else
				{
					routeParameters.push_back(i);

					auto [it, success] = routes.emplace(make_pair(routeParameters.back().baseRoute, smartPointer<BaseExecutor>(function())));

					auto node = settings.extract(i);

					node.key() = routeParameters.back().baseRoute;

					settings.insert(move(node));

					if (success)
					{
						if (it->second->getType() == BaseExecutor::executorType::stateful)
						{
							routes.erase(routeParameters.back().baseRoute);
						}
						else
						{
							it->second->init(j);
						}
					}
				}

				break;

			case utility::JSONSettingsParser::ExecutorSettings::loadType::dynamic:

				break;

			case utility::JSONSettingsParser::ExecutorSettings::loadType::none:
				throw exceptions::MissingLoadTypeException(j.name);

				break;
			}

			creator[j.name] = function;
		}

		executorsManager.init(assets, isCaching, pathToTemplates, move(routes), move(creator), move(settings), move(routeParameters));
	}
}
