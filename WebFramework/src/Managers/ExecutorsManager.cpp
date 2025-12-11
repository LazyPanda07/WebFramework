#include "ExecutorsManager.h"

#include <algorithm>
#include <ranges>
#include <format>

#include <Log.h>
#include <Exceptions/FileDoesNotExistException.h>
#include <Exceptions/DatabaseException.h>

#include "Exceptions/BadRequestException.h"
#include "Web/HTTPRequestImplementation.h"
#include "Utility/Sources.h"
#include "Utility/DynamicLibraries.h"
#include "Exceptions/MissingLoadTypeException.h"
#include "Exceptions/CantFindFunctionException.h"
#include "Utility/ExecutorsUtility.h"
#include "Managers/RuntimesManager.h"

namespace framework
{
	const std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& ExecutorsManager::StatefulExecutors::operator *() const
	{
		return executors;
	}

	std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& ExecutorsManager::StatefulExecutors::operator *()
	{
		return executors;
	}

	ExecutorsManager::StatefulExecutors::~StatefulExecutors()
	{
		for (const auto& [_, executor] : executors)
		{
			executor->destroy();
		}
	}

	bool ExecutorsManager::isFileRequest(std::string_view parameters)
	{
		size_t index = parameters.find('.');

		if (index == std::string_view::npos)
		{
			return false;
		}

		std::string_view fileExtension(parameters.begin() + index, parameters.end());

		return fileExtension.size() > 1 && std::ranges::all_of(fileExtension, [](char c) { return c != '/'; });
	}

	bool ExecutorsManager::isHeavyOperation(BaseExecutor* executor)
	{
		utility::ExecutorType executorType = executor->getType();

		return executorType == utility::ExecutorType::heavyOperationStateless ||
			executorType == utility::ExecutorType::heavyOperationStateful;
	}

	void ExecutorsManager::parseRouteParameters(const std::string& parameters, HTTPRequestExecutors& request, std::vector<utility::RouteParameters>::iterator it)
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
				catch (const std::invalid_argument&)
				{
					throw exceptions::BadRequestException("Can't convert to int64_t"); // 400
				}
				catch (const std::out_of_range&)
				{
					throw exceptions::BadRequestException("Out of range of int64_t"); // 400
				}

				break;

			case utility::RouteParameters::RouteParametersType::doubleTypeIndex:
				try
				{
					requestImplementation.routeParameters[it->indices[i++]] = stod(parameters.substr(startParameter, endParameter - startParameter));
				}
				catch (const std::invalid_argument&)
				{
					throw exceptions::BadRequestException("Can't convert to double"); // 400
				}
				catch (const std::out_of_range&)
				{
					throw exceptions::BadRequestException("Out of range of double"); // 400
				}

				break;

			default:
				throw std::runtime_error("Wrong routeParametersType");
			}

			startParameter = endParameter + 1;
		}
		while (endParameter != std::string::npos);
	}

	BaseExecutor* ExecutorsManager::getOrCreateExecutor(std::string& parameters, HTTPRequestExecutors& request, StatefulExecutors& executors)
	{
		std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& statefulExecutors = *executors;

		auto executor = statefulExecutors.find(parameters);

		if (executor == statefulExecutors.end())
		{
			std::unique_lock<std::mutex> lock(checkExecutor);

			executor = routes.find(parameters);

			if (executor == routes.end())
			{
				auto executorSettings = settings.find(parameters);

				if (executorSettings == settings.end())
				{
					auto it = std::find_if(routeParameters.begin(), routeParameters.end(),
						[&parameters](const utility::RouteParameters& value) { return parameters.find(value.baseRoute) == 0; });

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
					executor = statefulExecutors.insert(routes.extract(executor)).position;
				}
			}
		}

		return executor->second.get();
	}

	bool ExecutorsManager::filterUserAgent(const std::string& parameters, const web::HeadersMap& headers, HTTPResponseExecutors& response) const
	{
		const std::string& executorUserAgentFilter = settings.at(parameters).userAgentFilter;

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

	std::unique_ptr<BaseExecutor> ExecutorsManager::createAPIExecutor(const std::string& name, std::string_view apiType) const
	{
		return runtime::RuntimesManager::get().getRuntime(utility::getExecutorAPIType(apiType)).createExecutor(name);
	}

	void ExecutorsManager::initCreators(const std::vector<std::string>& pathToSources)
	{
		std::vector<std::pair<utility::LoadSource, std::string>> sources = utility::loadSources(pathToSources);

		routes.reserve(settings.size());

		std::vector<std::pair<std::string, std::string>> nodes;

#ifdef __ANDROID__
		std::string webFrameworkSharedLibraryPath = "libWebFramework.so";
#else
		std::string webFrameworkSharedLibraryPath = utility::getPathToWebFrameworkSharedLibrary();
#endif

		for (const auto& [route, executorSettings] : settings)
		{
			std::optional<utility::LoadSource> creatorSource;
			utility::ExecutorAPIType type = utility::getExecutorAPIType(executorSettings.apiType);

			for (const auto& [source, sourcePath] : sources)
			{
				if (runtime::RuntimesManager::get().getRuntime(type).loadExecutor(executorSettings.name, source))
				{
					creatorSource = source;

					break;
				}
			}

			if (!creatorSource)
			{
				if (Log::isValid())
				{
					Log::error("Can't find creator for executor: {} with API: {}", "LogWebFrameworkInitialization", executorSettings.name, executorSettings.apiType);
				}

				throw std::runtime_error(std::format("Can't find creator for executor: {} with API: {}", executorSettings.name, executorSettings.apiType));
			}

			runtime::RuntimesManager::get().getRuntime(utility::getExecutorAPIType(executorSettings.apiType)).initializeWebFramework(*creatorSource, webFrameworkSharedLibraryPath.data());

			switch (executorSettings.executorLoadType)
			{
			case utility::JSONSettingsParser::ExecutorSettings::LoadType::initialization:
				if (route.find('{') == std::string::npos)
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

			case utility::JSONSettingsParser::ExecutorSettings::LoadType::dynamic:
				if (route.find('{') != std::string::npos)
				{
					routeParameters.push_back(route);

					nodes.emplace_back(route, routeParameters.back().baseRoute);
				}

				break;

			case utility::JSONSettingsParser::ExecutorSettings::LoadType::none:
				throw exceptions::MissingLoadTypeException(executorSettings.name);

				break;
			}
		}

		for (auto&& [route, executorSettings] : nodes)
		{
			auto node = settings.extract(route);

			node.key() = std::move(executorSettings);

			settings.insert(std::move(node));
		}
	}

	ExecutorsManager::ExecutorsManager
	(
		const json::JsonParser& configuration,
		const std::vector<std::string>& pathToSources,
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
		const utility::AdditionalServerSettings& additionalSettings,
		std::shared_ptr<threading::ThreadPool> threadPool
	) :
		settings(move(executorsSettings)),
		resources(make_shared<ResourceExecutor>(configuration, additionalSettings, threadPool)),
		userAgentFilter(additionalSettings.userAgentFilter),
		serverType(ExecutorsManager::types.at(configuration.get<json::JsonObject>(json_settings::webFrameworkObject)[json_settings::webServerTypeKey].get<std::string>()))
	{
		this->initCreators(pathToSources);
	}

	ExecutorsManager::ExecutorsManager(ExecutorsManager&& other) noexcept
	{
		(*this) = std::move(other);
	}

	ExecutorsManager& ExecutorsManager::operator = (ExecutorsManager&& other) noexcept
	{
		this->routes = move(other.routes);
		this->settings = move(other.settings);
		this->resources = move(other.resources);
		this->routeParameters = move(other.routeParameters);
		this->userAgentFilter = move(other.userAgentFilter);
		this->serverType = other.serverType;

		return *this;
	}

	std::optional<std::function<void(HTTPRequestExecutors&, HTTPResponseExecutors&)>> ExecutorsManager::service(HTTPRequestExecutors& request, HTTPResponseExecutors& response, StatefulExecutors& executors)
	{
		BaseExecutor* executor = this->getOrCreateExecutor(request, response, executors);

		if (!executor)
		{
			return std::nullopt;
		}

		void (BaseExecutor:: * method)(HTTPRequestExecutors&, HTTPResponseExecutors&) = BaseExecutor::getMethod(request.getMethod());

		if (serverType == WebServerType::threadPool && ExecutorsManager::isHeavyOperation(executor))
		{
			return std::bind(method, executor, std::placeholders::_1, std::placeholders::_2);
		}

		std::invoke(method, executor, request, response);

		return std::nullopt;
	}

	BaseExecutor* ExecutorsManager::getOrCreateExecutor(HTTPRequestExecutors& request, HTTPResponseExecutors& response, StatefulExecutors& executors)
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

			std::string parameters(request.getRawParameters());
			BaseExecutor* executor = nullptr;
			bool fileRequest = ExecutorsManager::isFileRequest(parameters);

			if (parameters.find('?') != std::string::npos)
			{
				parameters.resize(parameters.find('?'));
			}

			executor = this->getOrCreateExecutor(parameters, request, executors);

			if (!fileRequest && !executor)
			{
				if (resources->fileExist(parameters))
				{
					return resources.get();
				}

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
		catch (const std::out_of_range&)
		{
			if (Log::isValid())
			{
				Log::error("Out of range", "LogExecutor");
			}

			throw;
		}
		catch (const std::exception& e)
		{
			if (Log::isValid())
			{
				Log::error("Executor manager exception: {}", "LogExecutorsManager", e.what());
			}

			throw;
		}
	}

	std::shared_ptr<ResourceExecutor> ExecutorsManager::getResourceExecutor() const
	{
		return resources;
	}
}
