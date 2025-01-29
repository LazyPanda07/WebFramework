#include "ExecutorsManager.h"

#include "Log.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/BadRequestException.h"
#include "Exceptions/DatabaseException.h"

using namespace std;

namespace framework
{
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
		if (!executor)
		{
			throw exceptions::BadRequestException(); // 400
		}

		BaseExecutor::executorType executorType = executor->getType();

		return executorType == BaseExecutor::executorType::heavyOperationStateless ||
			executorType == BaseExecutor::executorType::heavyOperationStateful;
	}

	void ExecutorsManager::parseRouteParameters(const string& parameters, HTTPRequest& request, vector<utility::RouteParameters>::iterator it)
	{
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

			case utility::RouteParameters::routeParametersType::doubleTypeIndex:
				try
				{
					request.routeParameters[it->indices[i++]] = stod(parameters.substr(startParameter, endParameter - startParameter));
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

	BaseExecutor* ExecutorsManager::getExecutor(string& parameters, HTTPRequest& request, unordered_map<string, unique_ptr<BaseExecutor>>& statefulExecutors)
	{
		auto executor = statefulExecutors.find(parameters);

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
					unique_ptr<BaseExecutor>(static_cast<BaseExecutor*>(creators[executorSettings->second.name]()))
				).first;
				executor->second->init(executorSettings->second);

				BaseExecutor::executorType executorType = executor->second->getType();

				if (executorType == BaseExecutor::executorType::stateful || executorType == BaseExecutor::executorType::heavyOperationStateful)
				{
					executor = statefulExecutors.insert(routes.extract(executor)).position; //-V837 //-V823
				}
			}
		}

		return executor->second.get();
	}

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
		vector<utility::RouteParameters>&& routeParameters,
		string_view userAgentFilter
	)
	{
		const unordered_map<string_view, webServerType> types =
		{
			{ json_settings::multiThreadedWebServerTypeValue, webServerType::multiThreaded },
			{ json_settings::threadPoolWebServerTypeValue, webServerType::threadPool },
			{ json_settings::loadBalancerWebServerTypeValue, webServerType::loadBalancer },
			{ json_settings::proxyWebServerTypeValue, webServerType::proxy }
		};

		this->routes = move(routes);
		this->creators = move(creators);
		this->settings = move(settings);
		this->routeParameters = move(routeParameters);
		this->userAgentFilter = userAgentFilter;

		resources = make_shared<ResourceExecutor>(configuraion, assets, cachingSize, pathToTemplates);

		resources->init(utility::JSONSettingsParser::ExecutorSettings());

		serverType = types.at(configuraion.getObject(json_settings::webFrameworkObject).getString(json_settings::webServerTypeKey));
	}

	optional<function<void(HTTPRequest&, HTTPResponse&)>> ExecutorsManager::service(HTTPRequest& request, HTTPResponse& response, unordered_map<string, unique_ptr<BaseExecutor>>& statefulExecutors)
	{
		static const unordered_map<string, void(BaseExecutor::*)(HTTPRequest&, HTTPResponse&)> methods =
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
			if (userAgentFilter.size())
			{
				const web::HeadersMap& headers = request.parser.getHeaders();

				if (auto it = headers.find("User-Agent"); it != headers.end())
				{
					if (userAgentFilter != it->second)
					{
						if (Log::isValid())
						{
							Log::info("Wrong User-Agent: {}", "LogFilter", it->second);
						}

						resources->forbiddenError(response, nullptr);

						return {};
					}
				}
			}

			string parameters = request.getRawParameters();
			BaseExecutor* executor = nullptr;
			bool fileRequest = ExecutorsManager::isFileRequest(parameters);

			if (parameters.find('?') != string::npos)
			{
				parameters.resize(parameters.find('?'));
			}

			executor = this->getExecutor(parameters, request, statefulExecutors);

			if (!fileRequest && !executor)
			{
				throw exceptions::BadRequestException(); // 400
			}

			if (fileRequest && executor)
			{
				fileRequest = false;
			}

			void (BaseExecutor:: * method)(HTTPRequest&, HTTPResponse&) = methods.at(request.getMethod());

			if (serverType == webServerType::threadPool && (fileRequest ? false : ExecutorsManager::isHeavyOperation(executor)))
			{
				return bind(method, executor, placeholders::_1, placeholders::_2);
			}
			else
			{
				fileRequest ?
					invoke(method, resources.get(), request, response) :
					invoke(method, executor, request, response);
			}

			return {};
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
		catch (const exceptions::DatabaseException& e)
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
