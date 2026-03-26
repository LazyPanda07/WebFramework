#pragma once

#include <ThreadPool.h>

#include "Framework/WebFrameworkPlatform.h"

#include "Web/Servers/BaseWebServer.h"
#include "Config.h"
#include "Utility/TaskExecutorsSettings.h"

#ifdef __WITH_STACKTRACE__
#include "Utility/CrashHandler.h"
#endif

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
		static void parseAdditionalConfigs(const json::JsonObject& webFrameworkSettings, const std::filesystem::path& basePath, std::vector<std::string>& settingsPaths, std::vector<std::string>& loadSources);

		static std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings> createExecutorsSettings(const std::vector<std::string>& settingsPaths);

	private:
		utility::Config config;
		std::unique_ptr<BaseWebServer> server;
		std::exception** serverException;
		std::string webServerType;
#ifdef __WITH_STACKTRACE__
		utility::CrashHandler crashHandler;
#endif
	private:
		uint64_t parseLoggingFlags(const json::JsonObject& loggingSettings) const;

	private:
		void initAPIs(const json::JsonObject& webFrameworkSettings);

		void initLogging() const;

		void initExecutors(const json::JsonObject& webFrameworkSettings, std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>& executorSettings, std::vector<std::string>& pathToSources);

		void initTaskExecutors(const json::JsonObject& taskBrokerObject);

		void initHTTPS(const json::JsonObject& webFrameworkSettings) const;

		void initDatabase(const json::JsonObject& webFrameworkSettings);

		void initServer
		(
			const json::JsonObject& webFrameworkSettings,
			std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
			const std::vector<std::string>& pathToSources
		);

	public:
		WebFramework(const utility::Config& webFrameworkConfig);

		WebFramework(const std::filesystem::path& webFrameworkConfigPath);

		void start(bool wait = false, const std::function<void()>& onStartServer = []() {});
		
		void stop(bool wait = true);

		void kick(const std::string& ip) const;

		void updateSslCertificates();

		std::vector<std::string> getClientsIp() const;

		bool isServerRunning() const;

		const json::JsonParser& getCurrentConfiguration() const;

		~WebFramework();
	};
}
