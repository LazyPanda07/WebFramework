#include "BaseWebServer.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/CantLoadSourceException.h"
#include "Exceptions/CantFindFunctionException.h"
#include "Exceptions/MissingLoadTypeException.h"
#include "Utility/Memory.h"
#include "Utility/ExecutorCreator.h"

using namespace std;

namespace framework
{
	BaseWebServer::BaseWebServer(const json::JSONParser& configuration, const vector<utility::JSONSettingsParser>& parsers, const filesystem::path& assets, const string& pathToTemplates, uint64_t cachingSize, const string& ip, const string& port, DWORD timeout, const vector<string>& pathToSources)
	{
		unordered_map<string, smartPointer<BaseExecutor>> routes;
		unordered_map<string, utility::ExecutorCreator> creators;
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
			creators.reserve(settings.size());

			vector<pair<string, string>> nodes;

			for (const auto& [i, j] : settings)
			{
				utility::ExecutorCreator creator;

				for (const auto& source : sources)
				{
					if (void* (*ptr)() = reinterpret_cast<void* (*)()>(GetProcAddress(source, ("create" + j.name + "Instance").data())))
					{
						creator.setCreateFunction(ptr);

						break;
					}

					creator.setCreateFunction(reinterpret_cast<createBaseExecutorSubclassFunction>(GetProcAddress(source, ("create" + j.name + "Instance").data())));
				}

				if (!creator)
				{
					throw exceptions::CantFindFunctionException("create" + j.name + "Instance");
				}

				switch (j.executorLoadType)
				{
				case utility::JSONSettingsParser::ExecutorSettings::loadType::initialization:
					if (i.find('{') == string::npos)
					{
						auto [it, success] = routes.emplace(make_pair(i, smartPointer<BaseExecutor>(creator())));

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

						auto [it, success] = routes.emplace(make_pair(routeParameters.back().baseRoute, smartPointer<BaseExecutor>(creator())));

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

				creators.emplace(j.name, creator);
			}

			for (auto&& [i, j] : nodes)
			{
				auto node = settings.extract(i);

				node.key() = move(j);

				settings.insert(move(node));
			}

			executorsManager.init(configuration, assets, cachingSize, pathToTemplates, move(routes), move(creators), move(settings), move(routeParameters));

			resources = executorsManager.getResourceExecutor();
	}
}
