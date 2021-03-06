#include "JSONSettingsParser.h"

#include <fstream>

#include "Exceptions/FileDoesNotExistException.h"

using namespace std;

namespace framework
{
	namespace utility
	{
		string JSONSettingsParser::parseInlineParameter(const string& xmlLine)
		{
			return string(xmlLine.begin() + xmlLine.find('>') + 1, xmlLine.begin() + xmlLine.rfind('<'));
		}

		JSONSettingsParser::JSONSettingsParser(const string& XMLSettings)
		{
			ifstream in(XMLSettings);

			if (!in.is_open())
			{
				throw exceptions::FileDoesNotExistException(XMLSettings);
			}

			json::JSONParser parser;

			in >> parser;

			for (const auto& i : parser)
			{
				const auto& data = get<json::utility::jJsonStruct>(i->second)->data;
				const string& loadType = get<json::utility::jString>(data.at("loadType"));
				ExecutorSettings executorSettings;

				if (data.find("initParameters") != data.end())
				{
					executorSettings.initParams.data = move(get<json::utility::jJsonStruct>(data.at("initParameters"))->data);
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
