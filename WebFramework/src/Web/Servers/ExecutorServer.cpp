#include "ExecutorServer.h"

#include <Exceptions/CantFindFunctionException.h>
#include <Exceptions/MissingLoadTypeException.h>
#include <Utility/Sources.h>
#include <Utility/DynamicLibraries.h>
#include <Log.h>

namespace framework
{
	ExecutorServer::ExecutorServer
	(
		const json::JSONParser& configuration,
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
		const std::vector<std::string>& pathToSources,
		const utility::AdditionalServerSettings& additionalSettings,
		std::shared_ptr<threading::ThreadPool> threadPool
	) :
		additionalSettings(additionalSettings)
	{
		try
		{
			executorsManager = make_unique<ExecutorsManager>(configuration, pathToSources, move(executorsSettings), additionalSettings, threadPool);

			resources = executorsManager->getResourceExecutor();
		}
		catch (const std::exception& e)
		{
			if (Log::isValid())
			{
				Log::fatalError("Can't create server: {}", "LogExecutorServer", 2, e.what());
			}

			throw;
		}
		catch (...)
		{
			if (Log::isValid())
			{
				Log::fatalError("Something went wrong", "LogExecutorServer", 2);
			}

			throw;
		}
	}
}
