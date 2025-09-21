#include "JSONSettingsParser.h"

#include <fstream>

#include <JSONParser.h>
#include <Strings.h>

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/CantFindValueException.h"
#include "WebFrameworkCoreConstants.h"

namespace framework::utility
{
	JSONSettingsParser::ExecutorSettings::ExecutorSettings() :
		executorLoadType(LoadType::none)
	{

	}

	JSONSettingsParser::ExecutorSettings::ExecutorSettings(std::string_view name) :
		name(name),
		executorLoadType(LoadType::none)
	{

	}

	JSONSettingsParser::JSONSettingsParser(const std::string& JSONSettings)
	{
		std::ifstream in(JSONSettings);

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
			const std::string& loadType = data.getString(json_settings::loadTypeKey);
			ExecutorSettings executorSettings(name);

			data.tryGetObject(json_settings::initParametersKey, executorSettings.initParameters);
			data.tryGetString(json_settings::userAgentFilterKey, executorSettings.userAgentFilter);
			
			executorSettings.apiType = data.getString(json_settings::apiTypeKey);

			if (loadType == json_settings_values::initializationLoadTypeValue)
			{
				executorSettings.executorLoadType = ExecutorSettings::LoadType::initialization;
			}
			else if (loadType == json_settings_values::dynamicLoadTypeValue)
			{
				executorSettings.executorLoadType = ExecutorSettings::LoadType::dynamic;
			}
			else
			{
				throw std::runtime_error("Wrong loadType");
			}

			settings.try_emplace(::utility::strings::replaceAll(data.getString(json_settings::routeKey), " ", "%20"), std::move(executorSettings));
		}
	}

	const std::unordered_map<std::string, JSONSettingsParser::ExecutorSettings>& JSONSettingsParser::getSettings() const
	{
		return settings;
	}

	const JSONSettingsParser::ExecutorSettings& JSONSettingsParser::getExecutorSettings(const std::string& executorName) const
	{
		return settings.at(executorName);
	}
}
