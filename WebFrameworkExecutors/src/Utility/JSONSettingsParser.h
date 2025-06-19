#pragma once

#include "JSONUtility.h"

#include "ExecutorsConstants.h"

namespace framework::utility
{
	/// <summary>
	/// Parser for .json settings file
	/// </summary>
	class EXECUTORS_API JSONSettingsParser
	{
	public:
		/// <summary>
		/// Settings for executor
		/// </summary>
		struct EXECUTORS_API ExecutorSettings
		{
			enum class loadType
			{
				initialization,
				dynamic,
				none
			};

			json::utility::jsonObject initParameters;
			std::string name;
			std::string userAgentFilter;
			std::string apiType;
			loadType executorLoadType;

		public:
			ExecutorSettings();

			ExecutorSettings(std::string_view name);

			ExecutorSettings(const ExecutorSettings& other) = default;

			ExecutorSettings(ExecutorSettings&& other) noexcept = default;

			ExecutorSettings& operator =(const ExecutorSettings& other) = default;

			ExecutorSettings& operator =(ExecutorSettings&& other) noexcept = default;

			~ExecutorSettings() = default;
		};

	private:
		std::unordered_map<std::string, ExecutorSettings> settings;	//route - settings

	public:
		/// <summary>
		/// Construct JSONSettingsParser from .json file
		/// </summary>
		/// <param name="JSONSettings">path to .json settings file</param>
		/// <exception cref="file_manager::exceptions::FileDoesNotExistException"></exception>
		/// <exception cref="std::runtime_error"></exception>
		JSONSettingsParser(const std::string& JSONSettings);

		/// @brief Move constructor
		/// @param other Another JSONSettingsParser
		JSONSettingsParser(JSONSettingsParser&& other) noexcept = default;

		/// <summary>
		/// Get all settings from .json
		/// </summary>
		/// <returns>route - settings as map</returns>
		const std::unordered_map<std::string, ExecutorSettings>& getSettings() const;

		/// <summary>
		/// Get settings for specific executor
		/// </summary>
		/// <param name="executorName">executor class name</param>
		/// <returns></returns>
		const ExecutorSettings& getExecutorSettings(const std::string& executorName) const;

		~JSONSettingsParser() = default;
	};
}
