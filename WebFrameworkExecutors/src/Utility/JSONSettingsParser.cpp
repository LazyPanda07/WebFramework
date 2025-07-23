#include "JSONSettingsParser.h"

#include <fstream>

#include "JSONParser.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/CantFindValueException.h"
#include "Strings.h"
#include "WebFrameworkCoreConstants.h"

using namespace std;

namespace framework::utility
{
	JSONSettingsParser::ExecutorSettings::ExecutorSettings() :
		executorLoadType(LoadType::none)
	{

	}

	JSONSettingsParser::ExecutorSettings::ExecutorSettings(string_view name) :
		name(name),
		executorLoadType(LoadType::none)
	{

	}

	JSONSettingsParser::JSONSettingsParser(const string& JSONSettings)
	{
		ifstream in(JSONSettings);

		if (!in.is_open())
		{
			throw file_manager::exceptions::FileDoesNotExistException(JSONSettings);
		}

		json::JSONParser parser;

		in >> parser;

		in.close();

		for (const auto& [name, description] : parser)
		{
			const json::utility::jsonObject& data = get<json::utility::jsonObject>(description);
			const string& LoadType = data.getString(json_settings::loadTypeKey);
			ExecutorSettings executorSettings(name);

			data.tryGetObject(json_settings::initParametersKey, executorSettings.initParameters);
			data.tryGetString(json_settings::userAgentFilterKey, executorSettings.userAgentFilter);
			data.tryGetString(json_settings::apiTypeKey, executorSettings.apiType);

			if (LoadType == json_settings_values::initializationLoadTypeValue)
			{
				executorSettings.executorLoadType = ExecutorSettings::LoadType::initialization;
			}
			else if (LoadType == json_settings_values::dynamicLoadTypeValue)
			{
				executorSettings.executorLoadType = ExecutorSettings::LoadType::dynamic;
			}
			else
			{
				throw runtime_error("Wrong LoadType");
			}

			settings.try_emplace(::utility::strings::replaceAll(data.getString(json_settings::routeKey), " ", "%20"), move(executorSettings));
		}
	}

	const unordered_map<string, JSONSettingsParser::ExecutorSettings>& JSONSettingsParser::getSettings() const
	{
		return settings;
	}

	const JSONSettingsParser::ExecutorSettings& JSONSettingsParser::getExecutorSettings(const string& executorName) const
	{
		return settings.at(executorName);
	}
}
