#pragma once

#include "Framework/WebFrameworkPlatform.h"

#include "WebNetwork/WebServers/BaseWebServer.h"

#include "Config.h"

namespace framework
{
	/// <summary>
	/// Main class of framework
	/// <para>All initializations go through this class</para>
	/// </summary>
	class WEB_FRAMEWORK_API WebFramework
	{
	public:
		/**
		 * @brief Is server use HTTPS
		 * @return 
		 */
		static bool getUseHTTPS();

	private:
		static void parseAdditionalConfigs(const json::utility::jsonObject& webFrameworkSettings, const std::filesystem::path& basePath, std::vector<std::string>& settingsPaths, std::vector<std::string>& loadSources);

		static std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings> createExecutorsSettings(const std::vector<std::string>& settingsPaths);

	private:
		utility::Config config;
		std::unique_ptr<web::BaseTCPServer> server;

	private:
		uint64_t parseLoggingFlags(const json::utility::jsonObject& loggingSettings) const;

	private:
		void initLogging() const;

		void initExecutors(const json::utility::jsonObject& webFrameworkSettings, std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>& executorSettings, std::vector<std::string>& pathToSources);

		void initHTTPS(const json::utility::jsonObject& webFrameworkSettings) const;

		void initDatabase(const json::utility::jsonObject& webFrameworkSettings);

		void initServer
		(
			const json::utility::jsonObject& webFrameworkSettings,
			std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
			const std::vector<std::string>& pathToSources
		);

	public:
		WebFramework(const utility::Config& webFrameworkConfig);

		WebFramework(const std::filesystem::path& webFrameworkConfigPath);

		/**
		 * @brief Start server
		 * @param wait Wait until server stop
		 * @param onStartServer On start server callback
		 */
		void start(bool wait = false, const std::function<void()>& onStartServer = []() {});
		
		/**
		 * @brief Stop server
		 * @param wait Wait until server stop
		 */
		void stop(bool wait = true);

		/// <summary>
		/// Kick specific client
		/// </summary>
		/// <param name="ip">client's address</param>
		void kick(const std::string& ip) const;

		/// <summary>
		/// Get ip addresses of all currently connected clients
		/// </summary>
		/// <returns></returns>
		std::vector<std::string> getClientsIp() const;

		/// <summary>
		/// Is server running
		/// </summary>
		/// <returns>is server running</returns>
		bool isServerRunning() const;

		/// @brief Getter for currentConfiguration
		/// @return Get current running configuration JSON 
		const json::JSONParser& getCurrentConfiguration() const;

		~WebFramework() = default;
	};
}
