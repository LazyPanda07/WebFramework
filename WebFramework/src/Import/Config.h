#pragma once

#include "Import/WebFrameworkCore.h"

#include "JSONParser.h"

namespace framework
{
	namespace utility
	{
		/**
		 * @brief Config file representation
		 */
		class WEB_FRAMEWORK_API Config
		{
		private:
			json::JSONParser currentConfiguration;
			std::filesystem::path basePath;

		private:
			template<std::ranges::range T>
			Config& overrideConfigurationArray(std::string_view key, const T& value, bool recursive);

		private:
			Config() = default;

		public:
			/**
			 * @brief 
			 * @param configPath Path to *.json config file
			 */
			Config(const std::filesystem::path& configPath);

			/**
			 * @brief Create config from text
			 * @param serverConfiguration UTF8 encoded configuration
			 * @param applicationDirectory Relative to that path all *.json settings construct their own paths(assets path, templates path, ...)
			 * @return
			 */
			Config(std::string_view serverConfiguration, std::string_view applicationDirectory);

			Config(const Config&) = default;

			Config(Config&&) noexcept = default;

			Config& operator = (const Config&) = default;

			Config& operator = (Config&&) noexcept = default;

			/**
			 * @brief Override specific config value
			 * @param key Config key
			 * @param value Config value
			 * @param recursive Search recursively
			 * @return 
			 */
			Config& overrideConfiguration(std::string_view key, const json::utility::jsonObject::variantType& value, bool recursive = true);

			/**
			 * @brief Override specific config value
			 * @param key Config key
			 * @param value Config value
			 * @param recursive Search recursively
			 * @return
			 */
			Config& overrideConfiguration(std::string_view key, const std::vector<int64_t>& value, bool recursive = true);

			/**
			 * @brief Override specific config value
			 * @param key Config key
			 * @param value Config value
			 * @param recursive Search recursively
			 * @return
			 */
			Config& overrideConfiguration(std::string_view key, const std::vector<std::string>& value, bool recursive = true);

			/**
			 * @brief Override config file directory
			 * @param basePath New base path
			 * @return Self
			 */
			Config& overrideBasePath(const std::filesystem::path& basePath);

			/**
			 * @brief Get string from config
			 * @param key Config key
			 * @param recursive Search recursively
			 * @return Config string value
			 */
			const std::string& getConfigurationString(std::string_view key, bool recursive = true) const;

			/**
			 * @brief Get integer from config
			 * @param key Config key
			 * @param recursive Search recursively
			 * @return Config integer value
			 */
			int64_t getConfigurationInteger(std::string_view key, bool recursive = true) const;

			/**
			 * @brief Get boolean from config
			 * @param key Config key
			 * @param recursive Search recursively
			 * @return Config boolean value
			 */
			bool getConfigurationBoolean(std::string_view key, bool recursive = true) const;

			/**
			 * @brief Config file directory
			 * @return 
			 */
			const std::filesystem::path& getBasePath() const;

			/**
			 * @brief Get current config JSON string data
			 * @return 
			 */
			std::string getConfiguration() const;

			/**
			 * @brief Get raw config JSON string data
			 * @return 
			 */
			std::string_view getRawConfiguration() const;

			/**
			 * @brief Actual settings
			 * @return 
			 */
			const json::JSONParser& operator * () const;

			~Config() = default;
		};
	}
}
