#include "JSONSettingsParser.h"

#include <fstream>

#include <JsonParser.h>
#include <MapJsonIterator.h>
#include <Strings.h>
#include <Exceptions/FileDoesNotExistException.h>
#include <Exceptions/CantFindValueException.h>

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

		json::JsonParser parser;

		in >> parser;

		in.close();

		json::MapJsonIterator iterator(parser.getParsedData());

		for (const auto& [name, description] : iterator)
		{
			if (name == "$schema")
			{
				continue;
			}

			const std::string& loadType = description[json_settings::loadTypeKey].get<std::string>();
			ExecutorSettings executorSettings(name);

			description.tryGet<json::JsonObject>(json_settings::initParametersKey, executorSettings.initParameters);
			description.tryGet<std::string>(json_settings::userAgentFilterKey, executorSettings.userAgentFilter);
			
			executorSettings.apiType = description[json_settings::apiTypeKey].get<std::string>();

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

			settings.try_emplace(::utility::strings::replaceAll(description[json_settings::routeKey].get<std::string>(), " ", "%20"), std::move(executorSettings));
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
