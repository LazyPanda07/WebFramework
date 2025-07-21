#include "ExecutorsManager.h"

#include <algorithm>
#include <ranges>
#include <format>

#include "Log.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/BadRequestException.h"
#include "Exceptions/DatabaseException.h"
#include "WebNetwork/HTTPRequestImplementation.h"
#include <Utility/Sources.h>
#include <Utility/DynamicLibraries.h>
#include <Exceptions/MissingLoadTypeException.h>
#include <Exceptions/CantFindFunctionException.h>

#include <Executors/CXXExecutor.h>

using namespace std;

namespace framework
{
	unordered_map<string, unique_ptr<BaseExecutor>>& ExecutorsManager::StatefulExecutors::operator* ()
	{
		return statefulExecutors;
	}

	ExecutorsManager::StatefulExecutors::~StatefulExecutors()
	{
		for (auto&& [_, executor] : statefulExecutors)
		{
			executor->destroy();
		}
	}

	bool ExecutorsManager::isFileRequest(string_view parameters)
	{
		size_t index = parameters.find('.');

		if (index == string_view::npos)
		{
			return false;
		}

		string_view fileExtension(parameters.begin() + index, parameters.end());

		return fileExtension.size() > 1 && ranges::all_of(fileExtension, [](char c) { return c != '/'; });
	}

	bool ExecutorsManager::isHeavyOperation(BaseExecutor* executor)
	{
		utility::ExecutorType ExecutorType = executor->getType();

		return ExecutorType == utility::ExecutorType::heavyOperationStateless ||
			ExecutorType == utility::ExecutorType::heavyOperationStateful;
	}

	void ExecutorsManager::parseRouteParameters(const string& parameters, HTTPRequest& request, vector<utility::RouteParameters>::iterator it)
	{
		size_t i = 0;
		size_t startParameter = it->baseRoute.size() + 1;
		size_t endParameter;
		HTTPRequestImplementation& requestImplementation = *static_cast<HTTPRequestImplementation*>(request.getImplementation());

		do
		{
			endParameter = parameters.find('/', startParameter);

			switch (static_cast<utility::RouteParameters::RouteParametersType>(it->parameters[it->indices[i]].index()))
			{
			case utility::RouteParameters::RouteParametersType::stringTypeIndex:
				requestImplementation.routeParameters[it->indices[i++]] = parameters.substr(startParameter, endParameter - startParameter);

				break;

			case utility::RouteParameters::RouteParametersType::integerTypeIndex:
				try
				{
					requestImplementation.routeParameters[it->indices[i++]] = stoll(parameters.substr(startParameter, endParameter - startParameter));
				}
				catch (const invalid_argument&)
				{
					throw exceptions::BadRequestException("Can't convert to int64_t"); // 400
				}
				catch (const out_of_range&)
				{
					throw exceptions::BadRequestException("Out of range of int64_t"); // 400
				}

				break;

			case utility::RouteParameters::RouteParametersType::doubleTypeIndex:
				try
				{
					requestImplementation.routeParameters[it->indices[i++]] = stod(parameters.substr(startParameter, endParameter - startParameter));
				}
				catch (const invalid_argument&)
				{
					throw exceptions::BadRequestException("Can't convert to double"); // 400
				}
				catch (const out_of_range&)
				{
					throw exceptions::BadRequestException("Out of range of double"); // 400
				}

				break;

			default:
				throw runtime_error("Wrong routeParametersType");
			}

			startParameter = endParameter + 1;
		} while (endParameter != string::npos);
	}

	BaseExecutor* ExecutorsManager::getOrCreateExecutor(string& parameters, HTTPRequest& request, unordered_map<string, unique_ptr<BaseExecutor>>& statefulExecutors)
	{
		auto executor = statefulExecutors.find(parameters);

		if (executor == statefulExecutors.end())
		{
			unique_lock<mutex> lock(checkExecutor);

			executor = routes.find(parameters);

			if (executor == routes.end())
			{
				auto executorSettings = settings.find(parameters);

				if (executorSettings == settings.end())
				{
					auto it = find_if(routeParameters.begin(), routeParameters.end(),
						[&parameters](const utility::RouteParameters& value) { return parameters.find(value.baseRoute) != string::npos; });

					if (it == routeParameters.end())
					{
						return nullptr;
					}

					executorSettings = settings.find(it->baseRoute);

					if (executorSettings == settings.end())
					{
						return nullptr;
					}

					ExecutorsManager::parseRouteParameters(parameters, request, it);

					parameters = it->baseRoute;
				}

				executor = routes.try_emplace
				(
					move(parameters),
					this->createAPIExecutor(executorSettings->second.name, executorSettings->second.apiType)
				).first;

				executor->second->init(executorSettings->second);

				utility::ExecutorType executorType = executor->second->getType();

				if (executorType == utility::ExecutorType::stateful || executorType == utility::ExecutorType::heavyOperationStateful)
				{
					executor = statefulExecutors.insert(routes.extract(executor)).position; //-V837 //-V823
				}
			}
		}

		return executor->second.get();
	}

	bool ExecutorsManager::filterUserAgent(const string& parameters, const web::HeadersMap& headers, HTTPResponse& response) const
	{
		const string& executorUserAgentFilter = settings.at(parameters).userAgentFilter;

		if (executorUserAgentFilter.size())
		{
			if (auto it = headers.find("User-Agent"); it != headers.end())
			{
				if (executorUserAgentFilter != it->second)
				{
					if (Log::isValid())
					{
						Log::info("Wrong User-Agent: {}", "LogFilter", it->second);
					}

					resources->forbiddenError(response, nullptr);

					return false;
				}
			}
			else
			{
				if (Log::isValid())
				{
					Log::info("No User-Agent provided", "LogFilter");
				}

				resources->forbiddenError(response, nullptr);

				return false;
			}
		}

		return true;
	}

	unique_ptr<BaseExecutor> ExecutorsManager::createAPIExecutor(const string& name, string_view apiType) const
	{
		static const unordered_map<string_view, function<unique_ptr<BaseExecutor>(const string& name)>> apiExecutors =
		{
			{ "", [this](const string& name) { return unique_ptr<BaseExecutor>(static_cast<BaseExecutor*>(creators.at(name)())); } },
			{ json_settings::cxxExecutorKey, [this](const string& name) { return make_unique<CXXExecutor>(creatorSources.at(name), creators.at(name)()); } }
		};

		return apiExecutors.at(apiType)(name);
	}

	ExecutorsManager::ExecutorsManager
	(
		const json::JSONParser& configuration,
		const vector<string>& pathToSources, 
		unordered_map<string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
		const utility::AdditionalServerSettings& additionalSettings,
		shared_ptr<threading::ThreadPool> threadPool
	) :
		settings(move(executorsSettings)),
		resources(make_shared<ResourceExecutor>(configuration, additionalSettings, threadPool)),
		userAgentFilter(additionalSettings.userAgentFilter),
		serverType(ExecutorsManager::types.at(configuration.getObject(json_settings::webFrameworkObject).getString(json_settings::webServerTypeKey)))
	{
		vector<HMODULE> sources = utility::loadSources(pathToSources);

		routes.reserve(settings.size());
		creators.reserve(settings.size());

		vector<pair<string, string>> nodes;
		string webFrameworkSharedLibraryPath = utility::getPathToWebFrameworkSharedLibrary();

		for (const auto& [route, executorSettings] : settings)
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

			creators.try_emplace(executorSettings.name, creator);
			creatorSources.try_emplace(executorSettings.name, creatorSource);

			if (InitializeWebFrameworkInExecutor initFunction = utility::load<InitializeWebFrameworkInExecutor>(creatorSource, "initializeWebFrameworkForExecutors"))
			{
				initFunction(webFrameworkSharedLibraryPath.data());
			}

			switch (executorSettings.executorLoadType)
			{
			case utility::JSONSettingsParser::ExecutorSettings::loadType::initialization:
				if (route.find('{') == string::npos)
				{
					auto [it, success] = routes.try_emplace
					(
						route,
						this->createAPIExecutor(executorSettings.name, executorSettings.apiType)
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
						this->createAPIExecutor(executorSettings.name, executorSettings.apiType)
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
		}

		for (auto&& [route, executorSettings] : nodes)
		{
			auto node = settings.extract(route);

			node.key() = move(executorSettings);

			settings.insert(move(node)); //-V837
		}
	}

	ExecutorsManager::ExecutorsManager(ExecutorsManager&& other) noexcept
	{
		(*this) = move(other);
	}

	ExecutorsManager& ExecutorsManager::operator = (ExecutorsManager&& other) noexcept
	{
		this->routes = move(other.routes);
		this->creators = move(other.creators);
		this->creatorSources = move(other.creatorSources);
		this->settings = move(other.settings);
		this->resources = move(other.resources);
		this->routeParameters = move(other.routeParameters);
		this->userAgentFilter = move(other.userAgentFilter);
		this->serverType = other.serverType;

		return *this;
	}

	optional<function<void(HTTPRequest&, HTTPResponse&)>> ExecutorsManager::service(HTTPRequest& request, HTTPResponse& response, unordered_map<string, unique_ptr<BaseExecutor>>& statefulExecutors)
	{
		BaseExecutor* executor = this->getOrCreateExecutor(request, response, statefulExecutors);

		if (!executor)
		{
			return nullopt;
		}

		void (BaseExecutor:: * method)(HTTPRequest&, HTTPResponse&) = BaseExecutor::getMethod(request.getMethod());

		if (serverType == WebServerType::threadPool && ExecutorsManager::isHeavyOperation(executor))
		{
			return bind(method, executor, placeholders::_1, placeholders::_2);
		}

		invoke(method, executor, request, response);

		return nullopt;
	}

	BaseExecutor* ExecutorsManager::getOrCreateExecutor(HTTPRequest& request, HTTPResponse& response, unordered_map<string, unique_ptr<BaseExecutor>>& statefulExecutors)
	{
		try
		{
			const web::HeadersMap& headers = request.getHeaders();

			if (userAgentFilter.size())
			{
				if (auto it = headers.find("User-Agent"); it != headers.end())
				{
					if (userAgentFilter != it->second)
					{
						if (Log::isValid())
						{
							Log::info("Wrong User-Agent: {}", "LogFilter", it->second);
						}

						resources->forbiddenError(response, nullptr);

						return nullptr;
					}
				}
				else
				{
					if (Log::isValid())
					{
						Log::info("No User-Agent provided", "LogFilter");
					}

					resources->forbiddenError(response, nullptr);

					return nullptr;
				}
			}

			string parameters(request.getRawParameters());
			BaseExecutor* executor = nullptr;
			bool fileRequest = ExecutorsManager::isFileRequest(parameters);

			if (parameters.find('?') != string::npos)
			{
				parameters.resize(parameters.find('?'));
			}

			executor = this->getOrCreateExecutor(parameters, request, statefulExecutors);

			if (!fileRequest && !executor)
			{
				throw exceptions::BadRequestException(); // 400
			}

			if (fileRequest && executor)
			{
				fileRequest = false;
			}

			if (fileRequest)
			{
				return resources.get();
			}
			else if (executor)
			{
				if (this->filterUserAgent(parameters, headers, response))
				{
					return executor;
				}
				else
				{
					resources->forbiddenError(response, nullptr);
				}
			}
			else
			{
				throw exceptions::BadRequestException(); // 400
			}

			return nullptr;
		}
		catch (const exceptions::BaseExecutorException& e)
		{
			if (Log::isValid())
			{
				Log::error("Executor exception: {}", "LogExecutor", e.what());
			}

			throw;
		}
		catch (const file_manager::exceptions::FileDoesNotExistException& e)
		{
			if (Log::isValid())
			{
				Log::error("File request exception. {}", "LogExecutor", e.what());
			}

			throw;
		}
		catch (const database::exception::DatabaseException& e)
		{
			if (Log::isValid())
			{
				Log::error("Database exception: {}", "LogWebFrameworkDatabase", e.what());
			}

			throw;
		}
		catch (const out_of_range&)
		{
			if (Log::isValid())
			{
				Log::error("Out of range", "LogExecutor");
			}

			throw;
		}
		catch (const exception& e)
		{
			if (Log::isValid())
			{
				Log::error("Executor manager exception: {}", "LogExecutorsManager", e.what());
			}

			throw;
		}
	}

	shared_ptr<ResourceExecutor> ExecutorsManager::getResourceExecutor() const
	{
		return resources;
	}
}
