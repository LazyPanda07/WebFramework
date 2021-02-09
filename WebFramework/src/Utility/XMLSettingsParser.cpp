#include "XMLSettingsParser.h"

#include <fstream>

#include "Exceptions/FileDoesNotExistException.h"

using namespace std;

namespace framework
{
	namespace utility
	{
		string XMLSettingsParser::parseInlineParameter(const string& xmlLine)
		{
			return string(xmlLine.begin() + xmlLine.find('>') + 1, xmlLine.begin() + xmlLine.rfind('<'));
		}

		XMLSettingsParser::XMLSettingsParser(const string& XMLSettings)
		{
			using namespace xml_settings_tags;

			ifstream in(XMLSettings);
			bool startExecutor = false;
			bool startInitParams = false;
			string line;
			string executorName;
			string route;
			string loadType;
			string paramName;
			string paramValue;
			ExecutorSettings executor;

			if (!in.is_open())
			{
				throw exceptions::FileDoesNotExistException(XMLSettings);
			}

			while (getline(in, line))
			{
				switch (startExecutor)
				{
				case true:
					if (line.find(endExecutorTag) != string::npos)
					{
						startExecutor = false;

						executor.name = move(executorName);

						if (loadType == xml_settings_values::initializationLoadTypeValue)
						{
							executor.executorLoadType = ExecutorSettings::loadType::initialization;
						}
						else if (loadType == xml_settings_values::dynamicLoadTypeValue)
						{
							executor.executorLoadType = ExecutorSettings::loadType::dynamic;
						}
						else
						{
							executor.executorLoadType = ExecutorSettings::loadType::none;
						}

						settings.insert(make_pair(move(route), move(executor)));

						continue;
					}

					break;

				case false:
					if (line.find(startExecutorTag) != string::npos)
					{
						startExecutor = true;

						continue;
					}

					break;
				}

				switch (startInitParams)
				{
				case true:
					if (line.find(endInitParamTag) != string::npos)
					{
						startInitParams = false;

						executor.initParams.insert(make_pair(move(paramName), move(paramValue)));

						continue;
					}

					break;

				case false:
					if (line.find(startInitParamTag) != string::npos)
					{
						startInitParams = true;

						continue;
					}

					break;
				}

				if (line.find(startRouteTag) != string::npos && line.find(endRouteTag) != string::npos)
				{
					route = parseInlineParameter(line);
				}

				if (line.find(startExecutorNameTag) != string::npos && line.find(endExecutorNameTag) != string::npos)
				{
					executorName = parseInlineParameter(line);
				}

				if (line.find(startParamNameTag) != string::npos && line.find(endParamNameTag) != string::npos)
				{
					paramName = parseInlineParameter(line);
				}

				if (line.find(startParamValueTag) != string::npos && line.find(endParamValueTag) != string::npos)
				{
					paramValue = parseInlineParameter(line);
				}

				if (line.find(startLoadTypeTag) != string::npos && line.find(endLoadTypeTag) != string::npos)
				{
					loadType = parseInlineParameter(line);
				}
			}

			in.close();
		}

		const unordered_map<string, XMLSettingsParser::ExecutorSettings>& XMLSettingsParser::getSettings() const
		{
			return settings;
		}

		const XMLSettingsParser::ExecutorSettings& XMLSettingsParser::getExecutorSettings(const string& executorName) const
		{
			return settings.at(executorName);
		}
	}
}
