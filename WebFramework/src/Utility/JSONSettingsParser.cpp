#include "JSONSettingsParser.h"

#include <fstream>

#include "Exceptions/FileDoesNotExistException.h"

using namespace std;

namespace framework
{
	namespace utility
	{
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
				const auto& data = get<json::utility::jJsonStruct>(i->second)->data;
				const string& loadType = get<json::utility::jString>(data.at("loadType"));
				ExecutorSettings executorSettings;

				if (data.find("initParameters") != data.end())
				{
					executorSettings.initParameters.data = move(get<json::utility::jJsonStruct>(data.at("initParameters"))->data);
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

				settings.insert(make_pair(get<json::utility::jString>(data.at("route")), move(executorSettings)));
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
