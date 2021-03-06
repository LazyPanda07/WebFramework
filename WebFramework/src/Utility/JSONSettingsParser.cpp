#include "JSONSettingsParser.h"

#include <fstream>

#include "Exceptions/FileDoesNotExistException.h"
#include "JSONParser.h"

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
