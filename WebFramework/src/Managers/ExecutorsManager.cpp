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
#include "Runtimes/PythonRuntime.h"
#include "Runtimes/DotNetRuntime.h"

#include "Executors/CXXExecutor.h"
#include "Executors/CCExecutor.h"

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

	void ExecutorsManager::callInitFunction(const utility::LoadSource& creatorSource, std::string_view webFrameworkSharedLibraryPath, std::string_view apiType)
	{
		utility::ExecutorAPIType type = utility::getExecutorAPIType(apiType);

		switch (type)
		{
		case framework::utility::ExecutorAPIType::cc:
			utility::load<InitializeWebFrameworkInExecutor>(std::get<HMODULE>(creatorSource), "initializeWebFrameworkCC")(webFrameworkSharedLibraryPath.data());

			break;

		case framework::utility::ExecutorAPIType::cxx:
			utility::load<InitializeWebFrameworkInExecutor>(std::get<HMODULE>(creatorSource), "initializeWebFrameworkCXX")(webFrameworkSharedLibraryPath.data());

			break;

		case framework::utility::ExecutorAPIType::python:
		case framework::utility::ExecutorAPIType::csharp:
			runtime::RuntimesManager::get().getRuntime(type).initializeWebFramework(webFrameworkSharedLibraryPath.data());

			break;

		default:
			throw std::runtime_error("Wrong api type name");
		}
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
		static const std::unordered_map<std::string_view, std::function<std::unique_ptr<BaseExecutor>(const std::string& name)>> apiExecutors =
		{
			{ json_settings::cxxExecutorKey, [this](const std::string& name) { return std::make_unique<CXXExecutor>(std::get<HMODULE>(creatorSources.at(name)), creators.at(name)()); } },
			{ json_settings::ccExecutorKey, [this](const std::string& name) { return std::make_unique<CCExecutor>(std::get<HMODULE>(creatorSources.at(name)), creators.at(name)(), name); } },
#ifdef __WITH_PYTHON_EXECUTORS__
			{ json_settings::pythonExecutorKey, [](const std::string& name) { return runtime::RuntimesManager::get().getRuntime<runtime::PythonRuntime>().createExecutor(name); } },
#endif
#ifdef __WITH_DOT_NET_EXECUTORS__
			{ json_settings::csharpExecutorKey, [](const std::string& name) { return runtime::RuntimesManager::get().getRuntime<runtime::DotNetRuntime>().createExecutor(name); } },
#endif
		};

		if (auto it = apiExecutors.find(apiType); it != apiExecutors.end())
		{
			return (it->second)(name);
		}

		throw std::runtime_error(std::format("Can't find executor type for {}", apiType));

		return nullptr;
	}

	void ExecutorsManager::initCreators(const std::vector<std::string>& pathToSources)
	{
		std::vector<std::pair<utility::LoadSource, std::string>> sources = utility::loadSources(pathToSources);

		routes.reserve(settings.size());
		creators.reserve(settings.size());

		std::vector<std::pair<std::string, std::string>> nodes;

#ifdef __ANDROID__
		std::string webFrameworkSharedLibraryPath = "libWebFramework.so";
#else
		std::string webFrameworkSharedLibraryPath = utility::getPathToWebFrameworkSharedLibrary();
#endif

		for (const auto& [route, executorSettings] : settings)
		{
			CreateExecutorFunction creator = nullptr;
			utility::LoadSource creatorSource;
			std::string apiType;
			utility::ExecutorAPIType type = utility::getExecutorAPIType(executorSettings.apiType);

			std::ranges::transform(executorSettings.apiType, std::back_inserter(apiType), [](char c) -> char { return toupper(c); });

			for (const auto& [source, sourcePath] : sources)
			{
				bool found = false;

				if (executorSettings.apiType == json_settings::cxxExecutorKey || executorSettings.apiType == json_settings::ccExecutorKey)
				{
					HMODULE module = std::get<HMODULE>(source);
					std::string creatorFunctionName = std::format("create{}{}Instance", executorSettings.name, apiType);

					if (creator = utility::load<CreateExecutorSignature>(module, creatorFunctionName))
					{
						creatorSource = module;

						if (Log::isValid())
						{
							Log::info("Found {} in {}", "LogWebFrameworkInitialization", creatorFunctionName, sourcePath.empty() ? "current" : sourcePath);
						}

						found = true;
					}
				}
				else if (executorSettings.apiType == json_settings::pythonExecutorKey || executorSettings.apiType == json_settings::csharpExecutorKey)
				{
					found = runtime::RuntimesManager::get().getRuntime(type).loadExecutor(executorSettings.name, source);

					if (found)
					{
						creator = CreateExecutorFunction(reinterpret_cast<void* (*)()>(1));
					}
				}
				else
				{
					throw std::runtime_error("Wrong visit module type");
				}

				if (found)
				{
					break;
				}
			}

			if (!creator)
			{
				if (Log::isValid())
				{
					Log::error("Can't find creator function create{}{}Instance for executor {}.", "LogWebFrameworkInitialization", executorSettings.name, apiType, executorSettings.name);
				}

				throw exceptions::CantFindFunctionException(std::format("create{}{}Instance", executorSettings.name, apiType));
			}

			creators.try_emplace(executorSettings.name, creator);
			creatorSources.try_emplace(executorSettings.name, creatorSource);

			ExecutorsManager::callInitFunction(creatorSource, webFrameworkSharedLibraryPath, executorSettings.apiType);

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

			node.key() = move(executorSettings);

			settings.insert(move(node));
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
		this->creators = move(other.creators);
		this->creatorSources = move(other.creatorSources);
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
