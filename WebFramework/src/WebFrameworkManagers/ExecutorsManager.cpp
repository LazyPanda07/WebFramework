#include "ExecutorsManager.h"

#include "Log.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/BadRequestException.h"

using namespace std;

namespace framework
{
	ExecutorsManager::ExecutorsManager() :
		serverType(webServerType::multiThreaded)
	{

	}

	ExecutorsManager::ExecutorsManager(ExecutorsManager&& other) noexcept
	{
		(*this) = move(other);
	}

	ExecutorsManager& ExecutorsManager::operator = (ExecutorsManager&& other) noexcept
	{
		this->routes = move(other.routes);
		this->creators = move(other.creators);
		this->settings = move(other.settings);
		this->resources = move(other.resources);

		return *this;
	}

	void ExecutorsManager::init
	(
		const json::JSONParser& configuraion,
		const filesystem::path& assets,
		uint64_t cachingSize,
		const filesystem::path& pathToTemplates,
		unordered_map<string, unique_ptr<BaseExecutor>>&& routes,
		unordered_map<string, createExecutorFunction>&& creators,
		unordered_map<string, utility::JSONSettingsParser::ExecutorSettings>&& settings,
		vector<utility::RouteParameters>&& routeParameters
	) noexcept
	{
		this->routes = move(routes);
		this->creators = move(creators);
		this->settings = move(settings);
		this->routeParameters = move(routeParameters);

		resources = make_shared<ResourceExecutor>(configuraion, assets, cachingSize, pathToTemplates);

		resources->init(utility::JSONSettingsParser::ExecutorSettings());

		const string& webServerType = configuraion.getObject(json_settings::webFrameworkObject).getString(json_settings::webServerTypeKey);

		if (webServerType == json_settings::multiThreadedWebServerTypeValue)
		{
			serverType = webServerType::multiThreaded;
		}
		else if (webServerType == json_settings::threadPoolWebServerTypeValue)
		{
			serverType = webServerType::threadPool;
		}
	}

	void ExecutorsManager::service(HTTPRequest& request, HTTPResponse& response, unordered_map<string, unique_ptr<BaseExecutor>>& statefulExecutors, optional<function<void(HTTPRequest&, HTTPResponse&)>>& threadPoolFunction)
	{
		static const unordered_map<string, decltype(&BaseExecutor::doGet)> methods =
		{
			{ "GET", &BaseExecutor::doGet },
			{ "POST", &BaseExecutor::doPost },
			{ "HEAD", &BaseExecutor::doHead },
			{ "PUT", &BaseExecutor::doPut },
			{ "DELETE", &BaseExecutor::doDelete },
			{ "PATCH", &BaseExecutor::doPatch },
			{ "OPTIONS",&BaseExecutor::doOptions },
			{ "TRACE", &BaseExecutor::doTrace },
			{ "CONNECT", &BaseExecutor::doConnect }
		};

		try
		{
			string parameters = request.getRawParameters();
			decltype(routes.find("")) executor;
			bool fileRequest = parameters.find('.') != string::npos;

			if (parameters.find('?') != string::npos)
			{
				parameters.resize(parameters.find('?'));
			}

			if (!fileRequest)
			{
				executor = statefulExecutors.find(parameters);

				if (executor == statefulExecutors.end())
				{
					unique_lock<mutex> scopeLock(checkExecutor);

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
								throw exceptions::NotImplementedException(); // 404
							}

							executorSettings = settings.find(it->baseRoute);

							if (executorSettings == settings.end())
							{
								throw exceptions::NotImplementedException(); // 404
							}

							size_t i = 0;
							size_t startParameter = it->baseRoute.size() + 1;
							size_t endParameter;

							do
							{
								endParameter = parameters.find('/', startParameter);

								switch (static_cast<utility::RouteParameters::routeParametersType>(it->parameters[it->indices[i]].index()))
								{
								case utility::RouteParameters::routeParametersType::stringTypeIndex:
									request.routeParameters[it->indices[i++]] = parameters.substr(startParameter, endParameter - startParameter);

									break;

								case utility::RouteParameters::routeParametersType::integerTypeIndex:
									try
									{
										request.routeParameters[it->indices[i++]] = stoll(parameters.substr(startParameter, endParameter - startParameter));
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
								}

								startParameter = endParameter + 1;
							} while (endParameter != string::npos);

							parameters = it->baseRoute;
						}

						executor = routes.insert
						(
							make_pair
							(
								move(parameters),
								unique_ptr<BaseExecutor>(static_cast<BaseExecutor*>(creators[executorSettings->second.name]()))
							)
						).first;
						executor->second->init(executorSettings->second);

						if (executor->second->getType() == BaseExecutor::executorType::stateful || executor->second->getType() == BaseExecutor::executorType::heavyOperationStateful)
						{
							executor = statefulExecutors.insert(routes.extract(executor)).position;
						}
					}
				}
			}

			void (BaseExecutor::* method)(HTTPRequest&, HTTPResponse&) = methods.at(request.getMethod());
			bool isThreadPoolTask = fileRequest ? false :
				(executor->second->getType() == BaseExecutor::executorType::heavyOperationStateless ||
					executor->second->getType() == BaseExecutor::executorType::heavyOperationStateful);

			if (serverType == webServerType::threadPool && isThreadPoolTask)
			{
				threadPoolFunction = bind(method, executor->second.get(), placeholders::_1, placeholders::_2);
			}
			else
			{
				fileRequest ?
					invoke(method, resources.get(), request, response) :
					invoke(method, executor->second.get(), request, response);
			}
		}
		catch (const exceptions::BaseExecutorException& e)
		{
			Log::error("Executor exception: {}", "LogExecutor", e.what());

			throw;
		}
		catch (const file_manager::exceptions::FileDoesNotExistException& e)
		{
			Log::error("File request exception. {}", "LogExecutor", e.what());

			throw;
		}
		catch (const out_of_range&)
		{
			Log::error("Out of range", "LogExecutor");

			throw;
		}
	}

	shared_ptr<ResourceExecutor> ExecutorsManager::getResourceExecutor()
	{
		return resources;
	}
}
