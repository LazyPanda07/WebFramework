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
			 * @brief Create config from text
			 * @param serverConfiguration UTF8 encoded configuration
			 * @return 
			 */
			static Config createConfig(std::string_view serverConfiguration, std::string_view sourcesPath);

		public:
			Config(const std::filesystem::path& configPath);

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
			Config& overrideConfiguration(std::string_view key, const json::utility::jsonObject::variantType& value, bool recursive = false);

			/**
			 * @brief Override specific config value
			 * @param key Config key
			 * @param value Config value
			 * @param recursive Search recursively
			 * @return
			 */
			Config& overrideConfiguration(std::string_view key, const std::vector<int64_t>& value, bool recursive = false);

			/**
			 * @brief Override specific config value
			 * @param key Config key
			 * @param value Config value
			 * @param recursive Search recursively
			 * @return
			 */
			Config& overrideConfiguration(std::string_view key, const std::vector<std::string>& value, bool recursive = false);

			/**
			 * @brief Config file directory
			 * @return 
			 */
			const std::filesystem::path& getBasePath() const;

			std::string getConfiguration() const;

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
