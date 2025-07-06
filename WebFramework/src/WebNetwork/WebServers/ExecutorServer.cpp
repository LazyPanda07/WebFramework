#include "ExecutorServer.h"

#include <Exceptions/CantFindFunctionException.h>
#include <Exceptions/MissingLoadTypeException.h>
#include <Utility/Sources.h>
#include <Utility/DynamicLibraries.h>
#include <Log.h>

using namespace std;

namespace framework
{
	ExecutorServer::ExecutorServer
	(
		const json::JSONParser& configuration,
		unordered_map<string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
		const vector<string>& pathToSources,
		const utility::AdditionalServerSettings& additionalSettings,
		threading::ThreadPool& threadPool
	) :
		additionalSettings(additionalSettings)
	{
		try
		{
			executorsManager = make_unique<ExecutorsManager>(configuration, pathToSources, move(executorsSettings), additionalSettings, threadPool);

			resources = executorsManager->getResourceExecutor();
		}
		catch (const exception& e)
		{
			if (Log::isValid())
			{
				Log::error("Can't create server: {}", "LogExecutorServer", e.what());
			}
		}
		catch (...)
		{
			cerr << "Something went wrong" << endl;

			if (Log::isValid())
			{
				Log::error("Something went wrong", "LogExecutorServer");
			}
		}
	}
}
