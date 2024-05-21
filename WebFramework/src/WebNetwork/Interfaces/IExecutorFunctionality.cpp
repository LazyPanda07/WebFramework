#include "IExecutorFunctionality.h"

#include "Exceptions/CantFindFunctionException.h"
#include "Exceptions/MissingLoadTypeException.h"
#include "Utility/Sources.h"

using namespace std;

namespace framework
{
	namespace interfaces
	{
		unordered_map<string, utility::JSONSettingsParser::ExecutorSettings> IExecutorFunctionality::createExecutorSettings(const vector<utility::JSONSettingsParser>& parsers)
		{
			unordered_map<string, utility::JSONSettingsParser::ExecutorSettings> result;

			for (const utility::JSONSettingsParser& parser : parsers)
			{
				for (const auto& [key, value] : parser.getSettings())
				{
					utility::JSONSettingsParser::ExecutorSettings executorSettings;

					executorSettings.name = value.name;
					executorSettings.executorLoadType = value.executorLoadType;

					if (value.initParameters.data.size())
					{
						executorSettings.initParameters = json::utility::jsonObject(value.initParameters);
					}

					result.insert(make_pair(key, move(executorSettings)));
				}
			}

			return result;
		}

		IExecutorFunctionality::IExecutorFunctionality
		(
			const json::JSONParser& configuration,
			const filesystem::path& assets,
			const filesystem::path& pathToTemplates,
			uint64_t cachingSize,
			const vector<utility::JSONSettingsParser>& parsers,
			const vector<string>& pathToSources
		)
		{
			unordered_map<string, unique_ptr<BaseExecutor>> routes;
			unordered_map<string, utility::ExecutorCreator> creators;
			vector<utility::RouteParameters> routeParameters;
			vector<HMODULE> sources = utility::loadSources(pathToSources);
			unordered_map<string, utility::JSONSettingsParser::ExecutorSettings> settings = IExecutorFunctionality::createExecutorSettings(parsers);

			routes.reserve(settings.size());
			creators.reserve(settings.size());

			vector<pair<string, string>> nodes;

			for (const auto& [i, j] : settings)
			{
				utility::ExecutorCreator creator;

				for (const auto& source : sources)
				{
					if (void* (*ptr)() = reinterpret_cast<void* (*)()>(utility::load(source, format("create{}Instance", j.name))))
					{
						creator.setCreateFunction(ptr);

						break;
					}

					creator.setCreateFunction(reinterpret_cast<createBaseExecutorSubclassFunction>(utility::load(source, format("create{}Instance", j.name))));
				}

				if (!creator)
				{
					throw exceptions::CantFindFunctionException(format("create{}Instance", j.name));
				}

				switch (j.executorLoadType)
				{
				case utility::JSONSettingsParser::ExecutorSettings::loadType::initialization:
					if (i.find('{') == string::npos)
					{
						auto [it, success] = routes.emplace(make_pair(i, unique_ptr<BaseExecutor>(creator())));

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

						auto [it, success] = routes.emplace(make_pair(routeParameters.back().baseRoute, unique_ptr<BaseExecutor>(creator())));

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
}
