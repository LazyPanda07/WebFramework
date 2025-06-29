#include "ExecutorServer.h"

#include "Exceptions/CantFindFunctionException.h"
#include "Exceptions/MissingLoadTypeException.h"
#include <Utility/Sources.h>
#include <Utility/DynamicLibraries.h>
#include "Log.h"

using namespace std;

namespace framework
{
	ExecutorServer::ExecutorServer
	(
		const json::JSONParser& configuration,
		unordered_map<string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
		const vector<string>& pathToSources,
		const utility::AdditionalServerSettings& additionalSettings
	) :
		additionalSettings(additionalSettings),
		executorsManager(configuration, pathToSources, move(executorsSettings), additionalSettings),
		resources(executorsManager.getResourceExecutor())
	{

	}
}
