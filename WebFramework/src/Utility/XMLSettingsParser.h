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
				enum class loadType
				{
					initialization,
					dynamic,
					none
				};

				std::unordered_map<std::string, std::string> initParams;
				std::string name;
				loadType executorLoadType;
			};

		private:
			std::unordered_map<std::string, ExecutorSettings> settings;	//route - settings

		private:
			static std::string parseInlineParameter(const std::string& xmlLine);

		public:
			XMLSettingsParser(const std::string& XMLSettings);

			//route - settings
			const std::unordered_map<std::string, ExecutorSettings>& getSettings() const;

			const ExecutorSettings& getExecutorSettings(const std::string& executorName) const;

			~XMLSettingsParser() = default;
		};
	}
}