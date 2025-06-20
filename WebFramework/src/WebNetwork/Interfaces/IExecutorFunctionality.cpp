#include "IExecutorFunctionality.h"

#include "Exceptions/CantFindFunctionException.h"
#include "Exceptions/MissingLoadTypeException.h"
#include "Utility/Sources.h"
#include "Log.h"

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
					result.try_emplace(key, value);
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
			const vector<string>& pathToSources,
			const utility::AdditionalServerSettings& additionalSettings
		) :
			additionalSettings(additionalSettings)
		{
			unordered_map<string, unique_ptr<BaseExecutor>> routes;
			unordered_map<string, CreateExecutorFunction> creators;
			unordered_map<string, HMODULE> creatorSources;
			vector<utility::RouteParameters> routeParameters;
			unordered_map<string, utility::JSONSettingsParser::ExecutorSettings> settings = IExecutorFunctionality::createExecutorSettings(parsers);
			vector<HMODULE> sources = utility::loadSources(pathToSources);

			routes.reserve(settings.size());
			creators.reserve(settings.size());

			vector<pair<string, string>> nodes;

			for (const auto& [i, j] : settings)
			{
				CreateExecutorFunction creator = nullptr;
				HMODULE creatorSource = nullptr;

				for (const HMODULE& source : sources)
				{
					if (creator = utility::load<CreateExecutorFunction>(source, format("create{}Instance", j.name)))
					{
						creatorSource = source;

						break;
					}
				}

				if (!creator)
				{
					if (Log::isValid())
					{
						Log::error("Can't find creator for executor {}", "LogWebFrameworkInitialization", j.name);
					}

					throw exceptions::CantFindFunctionException(format("create{}Instance", j.name));
				}

				switch (j.executorLoadType)
				{
				case utility::JSONSettingsParser::ExecutorSettings::loadType::initialization:
					if (i.find('{') == string::npos)
					{
						auto [it, success] = routes.try_emplace
						(
							i,
							unique_ptr<BaseExecutor>(static_cast<BaseExecutor*>(creator()))
						);

						if (success)
						{
							if (it->second->getType() == utility::ExecutorType::stateful || it->second->getType() == utility::ExecutorType::heavyOperationStateful)
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

						auto [it, success] = routes.try_emplace
						(
							routeParameters.back().baseRoute,
							unique_ptr<BaseExecutor>(static_cast<BaseExecutor*>(creator()))
						);

						nodes.emplace_back(i, routeParameters.back().baseRoute);

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

						nodes.emplace_back(i, routeParameters.back().baseRoute);
					}

					break;

				case utility::JSONSettingsParser::ExecutorSettings::loadType::none:
					throw exceptions::MissingLoadTypeException(j.name);

					break;
				}

				creators.try_emplace(j.name, creator);
				creatorSources.try_emplace(j.name, creatorSource);
			}

			for (auto&& [i, j] : nodes)
			{
				auto node = settings.extract(i);

				node.key() = move(j);

				settings.insert(move(node)); //-V837
			}

			executorsManager.init(configuration, assets, cachingSize, pathToTemplates, move(routes), move(creators), move(settings), move(routeParameters), additionalSettings, move(creatorSources));

			resources = executorsManager.getResourceExecutor();
		}
	}
}
