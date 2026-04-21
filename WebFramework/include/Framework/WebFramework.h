#pragma once

#include <ThreadPool.h>
#include <Log.h>

#include "Framework/WebFrameworkPlatform.h"

#include "Web/Servers/BaseWebServer.h"
#include "Config.h"
#include "Utility/TaskExecutorsSettings.h"
#include "Managers/DatabasesManager.h"
#include "Managers/TaskBrokersManager.h"
#include "Managers/TaskExecutorsManager.h"

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
		class HttpsData
		{
		private:
			std::filesystem::path pathToCertificate;
			std::filesystem::path pathToKey;

		public:
			HttpsData();

			HttpsData(const HttpsData&) = delete;

			HttpsData(HttpsData&& other) = default;

			HttpsData& operator =(const HttpsData&) = delete;

			HttpsData& operator =(HttpsData&& other) = default;

			void setPathToCertificate(const std::filesystem::path& pathToCertificate);

			void setPathToKey(const std::filesystem::path& pathToKey);

			const std::filesystem::path& getPathToCertificate() const;

			const std::filesystem::path& getPathToKey() const;

			~HttpsData() = default;
		};

	private:
		static void parseAdditionalConfigs(const json::JsonObject& webFrameworkSettings, const std::filesystem::path& basePath, std::vector<std::string>& settingsPaths, std::vector<std::string>& loadSources);

	private:
		utility::Config config;
		std::unique_ptr<BaseWebServer> server;
		std::exception** serverException;
		std::string webServerType;
		std::optional<HttpsData> httpsData;
		DatabasesManager databasesManager;
		task_broker::TaskBrokersManager taskBrokerManager;
		task_broker::TaskExecutorsManager taskExecutorsManager;
#ifdef __WITH_STACKTRACE__
		utility::CrashHandler crashHandler;
#endif
	private:
		uint64_t parseLoggingFlags(const json::JsonObject& loggingSettings) const;

		Log::VerbosityLevel parseVerbosity(const json::JsonObject& loggingSettings) const;

	private:
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings> createExecutorsSettings(const std::vector<std::string>& settingsPaths);

	private:
		void initAPIs(const json::JsonObject& webFrameworkSettings);

		void initLogging() const;

		void initExecutors(const json::JsonObject& webFrameworkSettings, std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>& executorSettings, std::vector<std::string>& pathToSources);

		void initTaskExecutors(const json::JsonObject& taskBrokerObject);

		void initHTTPS(const json::JsonObject& webFrameworkSettings);

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

		bool isServerRunning() const;

		std::vector<std::string> getClientsIp() const;

		const json::JsonParser& getCurrentConfiguration() const;

		const std::optional<HttpsData>& getHttpsData() const;

		DatabasesManager& getDatabasesManager();

		task_broker::TaskBrokersManager& getTaskBrokerManager();

		task_broker::TaskExecutorsManager& getTaskExecutorsManager();

		~WebFramework();
	};
}
