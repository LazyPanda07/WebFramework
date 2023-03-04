#include "ExecutorsManager.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/BadRequestException.h"

using namespace std;

namespace framework
{
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

	void ExecutorsManager::init(const json::JSONParser& configuraion, const filesystem::path& assets, uint64_t cachingSize, const string& pathToTemplates, unordered_map<string, smartPointer<BaseExecutor>>&& routes, unordered_map<string, createBaseExecutorSubclassFunction>&& creators, unordered_map<string, utility::JSONSettingsParser::ExecutorSettings>&& settings, vector<utility::RouteParameters>&& routeParameters) noexcept
	{
		this->routes = move(routes);
		this->creators = move(creators);
		this->settings = move(settings);
		this->routeParameters = move(routeParameters);

		resources = make_shared<ResourceExecutor>(configuraion, assets, cachingSize, pathToTemplates);

		resources->init(utility::JSONSettingsParser::ExecutorSettings());

		const string& webServerType = configuraion.getString(json_settings::webServerTypeKey);

		if (webServerType == json_settings::multiThreadedWebServerTypeValue)
		{
			serverType = webServerType::multiThreaded;
		}
		else if (webServerType == json_settings::threadPoolWebServerTypeValue)
		{
			serverType = webServerType::threadPool;
		}
	}

	void ExecutorsManager::service(HTTPRequest&& request, HTTPResponse& response, unordered_map<string, smartPointer<BaseExecutor>>& statefulExecutors, optional<function<void(HTTPRequest&&, HTTPResponse&)>>& threadPoolFunction)
	{
		static const unordered_map<string, decltype(&BaseExecutor::doGet)> methods =
		{
			{ "GET", &BaseExecutor::doGet },
			{ "POST", &BaseExecutor::doPost },
			{ "HEAD", &BaseExecutor::doHead },
			{ "PUT", &BaseExecutor::doPut },
			{ "DELETE", &BaseExecutor::doDelete },
			{ "OPTIONS", &BaseExecutor::doOptions },
			{ "TRACE", &BaseExecutor::doTrace }
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
								[&parameters](const utility::RouteParameters& value) {return parameters.find(value.baseRoute) != string::npos; });

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

						executor = routes.insert(make_pair(move(parameters), smartPointer<BaseExecutor>(creators[executorSettings->second.name]()))).first;
						executor->second->init(executorSettings->second);

						if (executor->second->getType() == BaseExecutor::executorType::stateful || executor->second->getType() == BaseExecutor::executorType::heavyOperationStateful)
						{
							executor = statefulExecutors.insert(routes.extract(executor)).position;
						}
					}
				}
			}

			auto method = methods.at(request.getMethod());
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
					invoke(method, resources.get(), move(request), response) :
					invoke(method, executor->second.get(), move(request), response);
			}
		}
		catch (const exceptions::BaseExecutorException&)
		{
			throw;
		}
		catch (const file_manager::exceptions::FileDoesNotExistException& e)
		{
			Log::error("File request exception. {}", e.what());

			throw;
		}
		catch (const out_of_range&)
		{
			throw;
		}
	}

	shared_ptr<ResourceExecutor>& ExecutorsManager::getResourceExecutor()
	{
		return resources;
	}
}
