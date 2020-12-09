#include "ExecutorsManager.h"

#include "WebFrameworkConstants.h"
#include "Exceptions/FileDoesNotExistException.h"

using namespace std;

namespace framework
{
	void ExecutorsManager::init(const filesystem::path& assets, unordered_map<string, unique_ptr<BaseExecutor>>&& routes, unordered_map<string, createBaseExecutorSubclassFunction>&& creator, unordered_map<string, utility::XMLSettingsParser::ExecutorSettings>&& settings) noexcept
	{
		this->routes = move(routes);
		this->creator = move(creator);
		this->settings = move(settings);
		
		resources = make_unique<ResourceExecutor>(assets);

		resources->init(utility::XMLSettingsParser::ExecutorSettings());
	}

	void ExecutorsManager::service(HTTPRequest&& request, HTTPResponse& response)
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

			if(!fileRequest)
			{
				lock_guard<mutex> scopeLock(checkExecutor);

				executor = routes.find(parameters);

				if (executor == routes.end())
				{
					const utility::XMLSettingsParser::ExecutorSettings& executorSettings = settings.at(parameters);

					routes[parameters] = unique_ptr<BaseExecutor>(creator[executorSettings.name]());
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
