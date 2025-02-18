#include "JSONSettingsParser.h"

#include <iostream>

#include "JSONParser.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/CantFindValueException.h"
#include "Strings.h"

using namespace std;

namespace framework
{
	namespace utility
	{
		JSONSettingsParser::ExecutorSettings::ExecutorSettings() :
			executorLoadType(loadType::none)
		{

		}

		JSONSettingsParser::ExecutorSettings::ExecutorSettings(string_view name) :
			name(name),
			executorLoadType(loadType::none)
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
				const string& loadType = data.getString(json_settings::loadTypeKey);
				ExecutorSettings executorSettings(name);

				data.tryGetObject(json_settings::initParametersKey, executorSettings.initParameters);
				data.tryGetString(json_settings::userAgentFilterKey, executorSettings.userAgentFilter);

				if (loadType == json_settings_values::initializationLoadTypeValue)
				{
					executorSettings.executorLoadType = ExecutorSettings::loadType::initialization;
				}
				else if (loadType == json_settings_values::dynamicLoadTypeValue)
				{
					executorSettings.executorLoadType = ExecutorSettings::loadType::dynamic;
				}
				else
				{
					throw runtime_error("Wrong loadType");
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
}
