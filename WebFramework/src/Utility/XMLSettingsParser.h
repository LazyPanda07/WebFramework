#pragma once

#include <string>
#include <unordered_map>

#if defined(EXECUTOR_DLL) || defined(ALL_DLL)
#define WEB_FRAMEWORK_API __declspec(dllexport)
#else
#define WEB_FRAMEWORK_API
#endif // EXECUTOR_DLL

namespace framework
{
	namespace utility
	{
		/// <summary>
		/// Parser for .xml settings file
		/// </summary>
		class WEB_FRAMEWORK_API XMLSettingsParser
		{
		public:
			/// <summary>
			/// Settings for executor
			/// </summary>
			struct WEB_FRAMEWORK_API ExecutorSettings
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
			/// <summary>
			/// Parse inline parameters https://github.com/LazyPanda07/WebFramework/wiki/WebFramework-markup
			/// </summary>
			/// <param name="xmlLine">parameter with tags</param>
			/// <returns>parameter without tags</returns>
			static std::string parseInlineParameter(const std::string& xmlLine);

		public:
			/// <summary>
			/// Construct XMLSettingsParser from .xml file
			/// </summary>
			/// <param name="XMLSettings">path to .xml settings file</param>
			/// <exception cref="framework::exceptions::FileDoesNotExistException"></exception>
			XMLSettingsParser(const std::string& XMLSettings);

			/// <summary>
			/// Get all settings from .xml
			/// </summary>
			/// <returns>route - settings as map</returns>
			const std::unordered_map<std::string, ExecutorSettings>& getSettings() const;

			/// <summary>
			/// Get settings for specific executor
			/// </summary>
			/// <param name="executorName">executor class name</param>
			/// <returns></returns>
			const ExecutorSettings& getExecutorSettings(const std::string& executorName) const;

			~XMLSettingsParser() = default;
		};
	}
}
