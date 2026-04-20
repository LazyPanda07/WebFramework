#include "Managers/ExecutorsManager.h"

#include <algorithm>
#include <ranges>
#include <format>

#include <Log.h>
#include <Exceptions/FileDoesNotExistException.h>
#include <Exceptions/DatabaseException.h>

#include "Exceptions/BadRequestException.h"
#include "Web/HttpRequestImplementation.h"
#include "Utility/Sources.h"
#include "Utility/DynamicLibraries.h"
#include "Exceptions/MissingLoadTypeException.h"
#include "Exceptions/CantFindFunctionException.h"
#include "Utility/ExecutorsUtility.h"
#include "Managers/RuntimesManager.h"
#include "Utility/Utils.h"

namespace framework
{
	const std::unordered_map<std::string, std::unique_ptr<Executor>>& ExecutorsManager::StatefulExecutors::operator *() const
	{
		return executors;
	}

	std::unordered_map<std::string, std::unique_ptr<Executor>>& ExecutorsManager::StatefulExecutors::operator *()
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

	bool ExecutorsManager::isHeavyOperation(Executor* executor)
	{
		utility::ExecutorType executorType = executor->getType();

		return executorType == utility::ExecutorType::heavyOperationStateless ||
			executorType == utility::ExecutorType::heavyOperationStateful;
	}

	void ExecutorsManager::parseRouteParameters(const std::string& parameters, interfaces::IHttpRequest& request, std::vector<utility::RouteParameters>::iterator it)
	{
		size_t i = 0;
		size_t startParameter = it->baseRoute.size() + 1;
		size_t endParameter;
		HttpRequestImplementation& requestImplementation = *static_cast<HttpRequestImplementation*>(&request);

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
					requestImplementation.routeParameters[it->indices[i++]] = std::stoll(parameters.substr(startParameter, endParameter - startParameter));
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
					requestImplementation.routeParameters[it->indices[i++]] = std::stod(parameters.substr(startParameter, endParameter - startParameter));
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

	Executor* ExecutorsManager::getOrCreateExecutor(std::string& parameters, interfaces::IHttpRequest& request, StatefulExecutors& executors)
	{
		std::unordered_map<std::string, std::unique_ptr<Executor>>& statefulExecutors = *executors;

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
					this->createApiExecutor(executorSettings->second.name, executorSettings->second.apiType)
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

	bool ExecutorsManager::filterUserAgent(const std::string& parameters, const web::HeadersMap& headers) const
	{
		const std::vector<std::string>& executorUserAgentFilter = settings.at(parameters).userAgentFilter;

		if (executorUserAgentFilter.size())
		{
			if (auto it = headers.find("User-Agent"); it != headers.end())
			{
				if (std::ranges::find(executorUserAgentFilter, it->second) == executorUserAgentFilter.end())
				{
					if (Log::isValid())
					{
						Log::info<logging::message::wrongUserAgent, logging::category::filter>(it->second);
					}

					return false;
				}
			}
			else
			{
				if (Log::isValid())
				{
					Log::info<logging::message::noUserAgent, logging::category::filter>();
				}

				return false;
			}
		}

		return true;
	}

	bool ExecutorsManager::filterJwt(const std::string& parameters, const web::HeadersMap& headers) const
	{
		if (settings.at(parameters).requireJwt)
		{
			if (auto it = headers.find("Authorization"); it != headers.end())
			{
				constexpr std::string_view bearer = "Bearer ";

				if (it->second.find(bearer) == std::string::npos)
				{
					if (Log::isValid())
					{
						Log::info<logging::message::noJwt, logging::category::filter>();
					}

					return false;
				}

				return true;
			}
		}

		return true;
	}

	std::unique_ptr<Executor> ExecutorsManager::createApiExecutor(const std::string& name, std::string_view apiType) const
	{
		return runtime::RuntimesManager::get().getRuntime(utility::getExecutorApiType(apiType)).createExecutor(name);
	}

	void ExecutorsManager::initCreators(const std::vector<std::string>& pathToSources)
	{
		std::vector<std::pair<utility::LoadSource, std::string>> sources = utility::loadSources(pathToSources);

		routes.reserve(settings.size());

		std::vector<std::pair<std::string, std::string>> nodes;

		for (auto& [route, executorSettings] : settings)
		{
			std::optional<utility::LoadSource> creatorSource;
			utility::ExecutorApiType type = utility::getExecutorApiType(executorSettings.apiType);

			executorSettings.resourceExecutor = resources;

			for (const auto& [source, sourcePath] : sources)
			{
				if (runtime::RuntimesManager::get().getRuntime(type).loadExecutor(executorSettings.name, route, source))
				{
					creatorSource = source;

					break;
				}
			}

			if (!creatorSource)
			{
				utility::logAndThrowException<logging::message::cantCreateApiExecutor, logging::category::executor>(executorSettings.name, executorSettings.apiType);
			}

			runtime::RuntimesManager::get().getRuntime(utility::getExecutorApiType(executorSettings.apiType)).initializeWebFramework(*creatorSource);

			switch (executorSettings.executorLoadType)
			{
			case utility::JSONSettingsParser::ExecutorSettings::LoadType::initialization:
				if (route.find('{') == std::string::npos)
				{
					auto [it, success] = routes.try_emplace
					(
						route,
						this->createApiExecutor(executorSettings.name, executorSettings.apiType)
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
						this->createApiExecutor(executorSettings.name, executorSettings.apiType)
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
		settings(std::move(executorsSettings)),
		resources(std::make_shared<ResourceExecutor>(configuration, additionalSettings, threadPool)),
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
		this->routes = std::move(other.routes);
		this->settings = std::move(other.settings);
		this->resources = std::move(other.resources);
		this->routeParameters = std::move(other.routeParameters);
		this->userAgentFilter = std::move(other.userAgentFilter);
		this->serverType = other.serverType;

		return *this;
	}

	std::optional<std::function<void(interfaces::IHttpRequest&, interfaces::IHttpResponse&)>> ExecutorsManager::service(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response, StatefulExecutors& executors)
	{
		Executor* executor = this->getOrCreateExecutor(request, response, executors);

		if (!executor)
		{
			return std::nullopt;
		}

		void (Executor:: * method)(interfaces::IHttpRequest&, interfaces::IHttpResponse&) = Executor::getMethod(request.getMethod());

		if (serverType == WebServerType::threadPool && ExecutorsManager::isHeavyOperation(executor))
		{
			return std::bind(method, executor, std::placeholders::_1, std::placeholders::_2);
		}

		std::invoke(method, executor, request, response);

		return std::nullopt;
	}

	Executor* ExecutorsManager::getOrCreateExecutor(interfaces::IHttpRequest& request, interfaces::IHttpResponse& response, StatefulExecutors& executors)
	{
		HttpRequestImplementation& requestImplementation = *static_cast<HttpRequestImplementation*>(&request);
		const web::HeadersMap& headers = requestImplementation.parser.getHeaders();

		if (userAgentFilter.size())
		{
			if (auto it = headers.find("User-Agent"); it != headers.end())
			{
				if (std::ranges::find(userAgentFilter, it->second) == userAgentFilter.end())
				{
					if (Log::isValid())
					{
						Log::info<logging::message::wrongUserAgent, logging::category::filter>(it->second);
					}

					resources->forbiddenError(response, nullptr);

					return nullptr;
				}
			}
			else
			{
				if (Log::isValid())
				{
					Log::info<logging::message::noUserAgent, logging::category::filter>();
				}

				resources->forbiddenError(response, nullptr);

				return nullptr;
			}
		}

		std::string parameters(request.getRawParameters());
		Executor* executor = nullptr;
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
			if (this->filterUserAgent(parameters, headers) && this->filterJwt(parameters, headers))
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

	std::shared_ptr<ResourceExecutor> ExecutorsManager::getResourceExecutor() const
	{
		return resources;
	}
}
