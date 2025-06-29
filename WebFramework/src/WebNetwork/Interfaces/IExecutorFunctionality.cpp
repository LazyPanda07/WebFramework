#include "IExecutorFunctionality.h"

#include "Exceptions/CantFindFunctionException.h"
#include "Exceptions/MissingLoadTypeException.h"
#include <Utility/Sources.h>
#include <Utility/DynamicLibraries.h>
#include "Log.h"

using namespace std;

namespace framework::interfaces
{
	IExecutorFunctionality::IExecutorFunctionality
	(
		const json::JSONParser& configuration,
		const filesystem::path& assets,
		const filesystem::path& pathToTemplates,
		uint64_t cachingSize,
		unordered_map<string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
		const vector<string>& pathToSources,
		const utility::AdditionalServerSettings& additionalSettings
	) :
		additionalSettings(additionalSettings)
	{
		unordered_map<string, unique_ptr<BaseExecutor>> routes;
		unordered_map<string, CreateExecutorFunction> creators;
		unordered_map<string, HMODULE> creatorSources;
		vector<utility::RouteParameters> routeParameters;
		vector<HMODULE> sources = utility::loadSources(pathToSources);

		routes.reserve(executorsSettings.size());
		creators.reserve(executorsSettings.size());

		vector<pair<string, string>> nodes;
		string webFrameworkSharedLibraryPath = utility::getPathToWebFrameworkSharedLibrary();

		for (const auto& [route, executorSettings] : executorsSettings)
		{
			CreateExecutorFunction creator = nullptr;
			HMODULE creatorSource = nullptr;

			for (const HMODULE& source : sources)
			{
				if (creator = utility::load<CreateExecutorFunction>(source, format("create{}Instance", executorSettings.name)))
				{
					creatorSource = source;

					break;
				}
			}

			if (!creator)
			{
				if (Log::isValid())
				{
					Log::error("Can't find creator for executor {}", "LogWebFrameworkInitialization", executorSettings.name);
				}

				throw exceptions::CantFindFunctionException(format("create{}Instance", executorSettings.name));
			}

			switch (executorSettings.executorLoadType)
			{
			case utility::JSONSettingsParser::ExecutorSettings::loadType::initialization:
				if (route.find('{') == string::npos)
				{
					auto [it, success] = routes.try_emplace
					(
						route,
						unique_ptr<BaseExecutor>(static_cast<BaseExecutor*>(creator()))
					);

					if (success)
					{
						if (it->second->getType() == utility::ExecutorType::stateful || it->second->getType() == utility::ExecutorType::heavyOperationStateful)
						{
							routes.erase(route);
						}
						else
						{
							it->second->init(executorSettings);
						}
					}
				}
				else
				{
					routeParameters.push_back(route);

					auto [it, success] = routes.try_emplace
					(
						routeParameters.back().baseRoute,
						unique_ptr<BaseExecutor>(static_cast<BaseExecutor*>(creator()))
					);

					nodes.emplace_back(route, routeParameters.back().baseRoute);

					if (success)
					{
						it->second->init(executorSettings);
					}
				}

				break;

			case utility::JSONSettingsParser::ExecutorSettings::loadType::dynamic:
				if (route.find('{') != string::npos)
				{
					routeParameters.push_back(route);

					nodes.emplace_back(route, routeParameters.back().baseRoute);
				}

				break;

			case utility::JSONSettingsParser::ExecutorSettings::loadType::none:
				throw exceptions::MissingLoadTypeException(executorSettings.name);

				break;
			}

			if (InitializeWebFrameworkInExecutor initFunction = utility::load<InitializeWebFrameworkInExecutor>(creatorSource, "initializeWebFrameworkForExecutors"))
			{
				initFunction(webFrameworkSharedLibraryPath.data());
			}

			creators.try_emplace(executorSettings.name, creator);
			creatorSources.try_emplace(executorSettings.name, creatorSource);
		}

		for (auto&& [route, executorSettings] : nodes)
		{
			auto node = executorsSettings.extract(route);

			node.key() = move(executorSettings);

			executorsSettings.insert(move(node)); //-V837
		}

		executorsManager.init(configuration, assets, cachingSize, pathToTemplates, move(routes), move(creators), move(executorsSettings), move(routeParameters), additionalSettings, move(creatorSources));

		resources = executorsManager.getResourceExecutor();
	}
}
