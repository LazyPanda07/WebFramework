#include "ExecutorsManager.h"

#include "WebFrameworkConstants.h"
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
		this->creator = move(other.creator);
		this->settings = move(other.settings);
		this->resources = move(other.resources);

		return *this;
	}

	void ExecutorsManager::init(const filesystem::path& assets, bool isCaching, const string& pathToTemplates, unordered_map<string, smartPointer<BaseExecutor>>&& routes, unordered_map<string, createBaseExecutorSubclassFunction>&& creator, unordered_map<string, utility::XMLSettingsParser::ExecutorSettings>&& settings, vector<utility::RouteParameters>&& routeParameters) noexcept
	{
		this->routes = move(routes);
		this->creator = move(creator);
		this->settings = move(settings);
		this->routeParameters = move(routeParameters);

		resources = make_unique<ResourceExecutor>(assets, isCaching, pathToTemplates);

		resources->init(utility::XMLSettingsParser::ExecutorSettings());
	}

	void ExecutorsManager::service(HTTPRequest&& request, HTTPResponse& response, unordered_map<string, smartPointer<BaseExecutor>>& statefulExecutors)
	{
		try
		{
			const string& method = request.getMethod();
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

							string tem;

							for (size_t i = it->baseRoute.size() + 1, j = 0; i < parameters.size(); i++)
							{
								if (i + 1 == parameters.size())
								{
									tem += parameters[i];
								}

								if (parameters[i] == '/' || i + 1 == parameters.size())
								{
									switch (static_cast<utility::RouteParameters::routeParametersType>(it->parameters[it->indices[j]].index()))
									{
									case utility::RouteParameters::routeParametersType::stringTypeIndex:
										request.routeParameters[it->indices[j++]] = move(tem);

										break;

									case utility::RouteParameters::routeParametersType::integerTypeIndex:
										try
										{
											request.routeParameters[it->indices[j++]] = stoll(tem);

											tem.clear();
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

									continue;
								}

								tem += parameters[i];
							}

							parameters = it->baseRoute;
						}

						executor = routes.insert(make_pair(move(parameters), smartPointer<BaseExecutor>(creator[executorSettings->second.name]()))).first;
						executor->second->init(executorSettings->second);

						if (executor->second->getType() == BaseExecutor::executorType::stateful)
						{
							executor = statefulExecutors.insert(routes.extract(executor)).position;
						}
					}
				}
			}

			if (method == getRequest)
			{
				fileRequest ? resources->doGet(move(request), response) : executor->second->doGet(move(request), response);
			}
			else if (method == postRequest)
			{
				fileRequest ? resources->doPost(move(request), response) : executor->second->doPost(move(request), response);
			}
			else if (method == headRequest)
			{
				fileRequest ? resources->doHead(move(request), response) : executor->second->doHead(move(request), response);
			}
			else if (method == putRequest)
			{
				fileRequest ? resources->doPut(move(request), response) : executor->second->doPut(move(request), response);
			}
			else if (method == deleteRequest)
			{
				fileRequest ? resources->doDelete(move(request), response) : executor->second->doDelete(move(request), response);
			}
			else if (method == optionsRequest)
			{
				fileRequest ? resources->doOptions(move(request), response) : executor->second->doOptions(move(request), response);
			}
			else if (method == traceRequest)
			{
				fileRequest ? resources->doTrace(move(request), response) : executor->second->doTrace(move(request), response);
			}
		}
		catch (const exceptions::BaseExecutorException&)
		{
			throw;
		}
		catch (const exceptions::FileDoesNotExistException&)
		{
			throw;
		}
		catch (const out_of_range&)
		{
			throw;
		}
	}

	smartPointer<ResourceExecutor>& ExecutorsManager::getResourceExecutor()
	{
		return resources;
	}
}
