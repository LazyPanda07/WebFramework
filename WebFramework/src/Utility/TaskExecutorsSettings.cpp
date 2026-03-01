#include "Utility/TaskExecutorsSettings.h"

#include <JsonArrayWrapper.h>

#include "Framework/WebFrameworkConstants.h"

namespace framework::utility
{
	TaskExecutorsSettings TaskExecutorsSettings::createTaskExecutorsSettings(const json::JsonObject& taskExecutorsArrayElement)
	{
		TaskExecutorsSettings result;

		result.sourcePath = taskExecutorsArrayElement[json_settings::taskExecutorsSourcePathKey].get<std::string>();
		result.classNames = json::utility::JsonArrayWrapper(taskExecutorsArrayElement[json_settings::taskExecutorsClassNamesKey].get<std::vector<json::JsonObject>>()).as<std::string>();
		result.api = taskExecutorsArrayElement[json_settings::taskExecutorsApiKey].get<std::string>();

		return result;
	}
}
