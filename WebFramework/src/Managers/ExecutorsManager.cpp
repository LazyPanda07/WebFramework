#include "ExecutorsManager.h"

using namespace std;

namespace framework
{
	void ExecutorsManager::init(unordered_map<string, unique_ptr<BaseExecutor>>&& routes, unordered_map<string, createBaseExecutorSubclassFunction>&& creator, unordered_map<string, utility::XMLSettingsParser::ExecutorSettings>&& settings) noexcept
	{
		this->routes = move(routes);
		this->creator = move(creator);
		this->settings = move(settings);
	}

	void ExecutorsManager::service(web::HTTPParser&& request, string& response)
	{
		try
		{
			const string& method = request.getMethod();
			string parameters = request.getParameters();
			decltype(routes.find("")) executor;

			parameters.resize(parameters.rfind('/') + 1);

			{
				lock_guard<mutex> scopeLock(checkExecutor);

				executor = routes.find(parameters);

				if (executor == routes.end())
				{
					const utility::XMLSettingsParser::ExecutorSettings& executorSettings = settings.at(parameters);

					routes[parameters] = unique_ptr<BaseExecutor>(creator[executorSettings.name]());
				}
			}

			if (method == "GET")
			{
				executor->second->doGet(move(request), response);
			}
			else if (method == "POST")
			{
				executor->second->doPost(move(request), response);
			}
		}
		catch (const exceptions::BaseExecutorException&)
		{
			throw;
		}
		catch (const out_of_range&)
		{
			throw;
		}
	}
}
