#include "XMLSettingsParser.h"

#include <fstream>

#include "WebFrameworkConstants.h"

using namespace std;

namespace framework
{
	namespace utility
	{
		XMLSettingsParser::XMLSettingsParser(const string& XMLSettings)
		{
			using namespace XMLSettingsTags;

			ifstream in(XMLSettings);
			bool startExecutor = false;
			bool startInitParams = false;
			string line;
			string executorName;
			string route;
			string paramName;
			string paramValue;
			ExecutorSettings executor;

			while (getline(in, line))
			{
				switch (startExecutor)
				{
				case true:
					if (line.find(endExecutorTag) != string::npos)
					{
						startExecutor = false;

						executor.route = move(route);

						settings.insert(make_pair(move(executorName), move(executor)));

						continue;
					}

					break;

				case false:
					if (line.find(startExecutorTag) != string::npos)
					{
						startExecutor = true;
					}

					continue;

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
					}

					continue;

					break;
				}

				if (line.find(startRouteTag) != string::npos && line.find(endRouteTag) != string::npos)
				{
					route = string(line.begin() + line.find('>'), line.begin() + line.rfind('<'));
				}

				if (line.find(startExecutorNameTag) != string::npos && line.find(endExecutorNameTag) != string::npos)
				{
					executorName = string(line.begin() + line.find('>'), line.begin() + line.rfind('<'));
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
