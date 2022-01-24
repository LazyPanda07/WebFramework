#include "JSONSettingsParser.h"

#include "Exceptions/FileDoesNotExistException.h"
#include "Exceptions/CantFindValueException.h"

using namespace std;

namespace framework
{
	namespace utility
	{
		JSONSettingsParser::ExecutorSettings::ExecutorSettings(ExecutorSettings&& other) noexcept :
			initParameters(move(other.initParameters)),
			name(move(other.name)),
			executorLoadType(other.executorLoadType)
		{

		}

		JSONSettingsParser::JSONSettingsParser(const string& JSONSettings)
		{
			ifstream in(JSONSettings);

			if (!in.is_open())
			{
				throw exceptions::FileDoesNotExistException(JSONSettings);
			}

			json::JSONParser parser;

			in >> parser;

			in.close();

			for (const auto& i : parser)
			{
				const auto& data = get<json::utility::jsonObject>(i->second);

				const string& loadType = data.getString("loadType");
				ExecutorSettings executorSettings;

				try
				{
					executorSettings.initParameters = data.getObject("initParameters");
				}
				catch (const json::exceptions::CantFindValueException&)
				{

				}

				executorSettings.name = i->first;
				
				if (loadType == json_settings_values::initializationLoadTypeValue)
				{
					executorSettings.executorLoadType = ExecutorSettings::loadType::initialization;
				}
				else if (loadType == json_settings_values::dynamicLoadTypeValue)
				{
					executorSettings.executorLoadType = ExecutorSettings::loadType::dynamic;
				}

				settings.insert(make_pair(data.getString("route"), move(executorSettings)));
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
