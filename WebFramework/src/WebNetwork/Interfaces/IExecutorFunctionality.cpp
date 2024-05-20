#include "IExecutorFunctionality.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/CantLoadSourceException.h"
#include "Exceptions/CantFindFunctionException.h"
#include "Exceptions/MissingLoadTypeException.h"

using namespace std;

static string makePathToSource(const string& pathToSource);

namespace framework
{
	namespace interfaces
	{
		vector<HMODULE> IExecutorFunctionality::loadSources(const vector<string>& pathToSources)
		{
			vector<HMODULE> result;

			result.reserve(pathToSources.size());

			for (const string& temp : pathToSources)
			{
				if (temp == json_settings::defaultLoadSourceValue)
				{
#ifdef __LINUX__
					result.push_back(dlopen(nullptr, RTLD_LAZY));
#else
					result.push_back(nullptr);
#endif

					continue;
				}

				string pathToSource = makePathToSource(temp);

				if (filesystem::exists(pathToSource))
				{
					HMODULE handle = nullptr;

#ifdef __LINUX__
					handle = dlopen(pathToSource.data(), RTLD_LAZY);
#else
					handle = LoadLibraryA(pathToSource.data());
#endif
					result.push_back(handle);
				}
				else
				{
					throw file_manager::exceptions::FileDoesNotExistException(pathToSource);
				}

				if (!result.back())
				{
					throw exceptions::CantLoadSourceException(pathToSource);
				}
			}

			return result;
		}

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
			vector<HMODULE> sources = IExecutorFunctionality::loadSources(pathToSources);
			unordered_map<string, utility::JSONSettingsParser::ExecutorSettings> settings = IExecutorFunctionality::createExecutorSettings(parsers);

			routes.reserve(settings.size());
			creators.reserve(settings.size());

			vector<pair<string, string>> nodes;
			auto load = [](HMODULE handle, string_view name)
				{
#ifdef __LINUX__
					return dlsym(handle, name.data());
#else
					return GetProcAddress(handle, name.data());
#endif
				};

			for (const auto& [i, j] : settings)
			{
				utility::ExecutorCreator creator;

				for (const auto& source : sources)
				{
					if (void* (*ptr)() = reinterpret_cast<void* (*)()>(load(source, format("create{}Instance", j.name))))
					{
						creator.setCreateFunction(ptr);

						break;
					}

					creator.setCreateFunction(reinterpret_cast<createBaseExecutorSubclassFunction>(load(source, format("create{}Instance", j.name))));
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

string makePathToSource(const string& pathToSource)
{
#ifdef __LINUX__
	filesystem::path temp(pathToSource);
	filesystem::path parent = temp.parent_path();
	filesystem::path fileName = temp.filename();

	return format("{}/lib{}.so", parent.string(), fileName.string());
#else
	return format("{}.dll", pathToSource);
#endif
}
