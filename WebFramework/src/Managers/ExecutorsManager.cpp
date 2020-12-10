#include "ExecutorsManager.h"

#include "WebFrameworkConstants.h"
#include "Exceptions/FileDoesNotExistException.h"

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

	void ExecutorsManager::init(const filesystem::path& assets, bool isCaching, unordered_map<string, unique_ptr<BaseExecutor>>&& routes, unordered_map<string, createBaseExecutorSubclassFunction>&& creator, unordered_map<string, utility::XMLSettingsParser::ExecutorSettings>&& settings) noexcept
	{
		this->routes = move(routes);
		this->creator = move(creator);
		this->settings = move(settings);

		resources = make_unique<ResourceExecutor>(assets, isCaching);

		resources->init(utility::XMLSettingsParser::ExecutorSettings());
	}

	void ExecutorsManager::service(HTTPRequest&& request, HTTPResponse& response, unordered_map<string, unique_ptr<BaseExecutor>>& statefulExecutors)
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
					lock_guard<mutex> scopeLock(checkExecutor);

					executor = routes.find(parameters);

					if (executor == routes.end())
					{
						const utility::XMLSettingsParser::ExecutorSettings& executorSettings = settings.at(parameters);

						executor = routes.insert(make_pair(move(parameters), unique_ptr<BaseExecutor>(creator[executorSettings.name]()))).first;
						executor->second->init(executorSettings);

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

	unique_ptr<ResourceExecutor>& ExecutorsManager::getResourceExecutor()
	{
		return resources;
	}
}
