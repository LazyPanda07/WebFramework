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

			for (const string& pathToSource : pathToSources)
			{
				if (!filesystem::exists(pathToSource))
				{
					if (pathToSource == json_settings::defaultLoadSourceValue)
					{
						result.push_back(nullptr);

						continue;
					}
					else
					{
						throw file_manager::exceptions::FileDoesNotExistException(pathToSource);
					}
				}
				else
				{
					result.push_back(LoadLibraryA(pathToSource.data()));
				}

				if (!result.back())
				{
					throw exceptions::CantLoadSourceException(pathToSource);
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

				if (i.second.initParameters.data.size())
				{
					executorSettings.initParameters = json::utility::jsonObject(i.second.initParameters);
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
