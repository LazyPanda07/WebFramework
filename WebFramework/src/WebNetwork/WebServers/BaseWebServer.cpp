#include "BaseWebServer.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/CantLoadSourceException.h"
#include "Exceptions/CantFindFunctionException.h"
#include "Exceptions/MissingLoadTypeException.h"
#include "Utility/Memory.h"

using namespace std;

namespace framework
{
	BaseWebServer::BaseWebServer(const vector<utility::JSONSettingsParser>& parsers, const filesystem::path& assets, const string& pathToTemplates, bool isCaching, const string& ip, const string& port, DWORD timeout, const vector<string>& pathToSources)
	{
		unordered_map<string, smartPointer<BaseExecutor>> routes;
		unordered_map<string, createBaseExecutorSubclassFunction> creator;
		unordered_map<string, utility::JSONSettingsParser::ExecutorSettings> settings;
		vector<utility::RouteParameters> routeParameters;
		vector<HMODULE> sources = [&pathToSources]() -> vector<HMODULE>
		{
			vector<HMODULE> result;

			result.reserve(pathToSources.size());

			for (const auto& i : pathToSources)
			{
				if (!filesystem::exists(i))
				{
					if (i == json::defaultLoadSourceValue)
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

		for (const auto& parser : parsers)
		{
			const auto& jsonSettings = parser.getSettings();

			for (const auto& i : jsonSettings)
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
		}

		routes.reserve(settings.size());
		creator.reserve(settings.size());

		vector<pair<string, string>> nodes;

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
						if (it->second->getType() == BaseExecutor::executorType::stateful || it->second->getType() == BaseExecutor::executorType::heavyOperationStateful)
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

					nodes.push_back(make_pair(i, routeParameters.back().baseRoute));

					if (success)
					{
						it->second->init(j);
					}
				}

				break;

			case utility::JSONSettingsParser::ExecutorSettings::loadType::dynamic:
				if (i.find('{') != string::npos)
				{
					routeParameters.push_back(i);

					nodes.push_back(make_pair(i, routeParameters.back().baseRoute));
				}

				break;

			case utility::JSONSettingsParser::ExecutorSettings::loadType::none:
				throw exceptions::MissingLoadTypeException(j.name);

				break;
			}

			creator[j.name] = function;
		}

		for (auto&& [i, j] : nodes)
		{
			auto node = settings.extract(i);

			node.key() = move(j);

			settings.insert(move(node));
		}

		executorsManager.init(assets, isCaching, pathToTemplates, move(routes), move(creator), move(settings), move(routeParameters));

		resources = executorsManager.getResourceExecutor().get();
	}
}
