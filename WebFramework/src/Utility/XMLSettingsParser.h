#pragma once

#include <string>
#include <unordered_map>

namespace framework
{
	namespace utility
	{
		class XMLSettingsParser
		{
		public:
			struct ExecutorSettings
			{
				std::string route;
				std::unordered_map<std::string, std::string> initParams;
			};

		private:
			std::unordered_map<std::string, ExecutorSettings> settings;

		public:
			XMLSettingsParser(const std::string& XMLSettings);

			const std::unordered_map<std::string, ExecutorSettings>& getSettings() const;

			const ExecutorSettings& getExecutorSettings(const std::string& executorName) const;

			~XMLSettingsParser() = default;
		};
	}
}