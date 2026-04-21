#include "Framework/WebFramework.h"

#include <filesystem>
#include <random>

#ifdef __LINUX__
#include <signal.h>
#endif

#include <JsonArrayWrapper.h>
#include <MapJsonIterator.h>
#include <DatabaseUtility.h>

#include "Web/Servers/MultithreadedWebServer.h"
#include "Web/Servers/ThreadPoolWebServer.h"
#include "LoadBalancer/LoadBalancerServer.h"
#include "Utility/Sources.h"
#include "Proxy/ProxyServer.h"
#include "Utility/DynamicLibraries.h"
#include "Framework/WebFrameworkConstants.h"
#include "Managers/RuntimesManager.h"
#include "Managers/TaskExecutorsManager.h"
#include "Managers/TaskBrokersManager.h"
#include "Runtimes/CXXRuntime.h"
#include "Runtimes/CCRuntime.h"
#include "Runtimes/PythonRuntime.h"
#include "Runtimes/DotNetRuntime.h"
#include "TaskBroker/InternalTaskBroker.h"
#include "Utility/Utils.h"

namespace framework
{
	WebFramework::HttpsData::HttpsData()
	{
#ifdef __LINUX__
		signal(SIGPIPE, SIG_IGN);
#endif

		SSL_library_init();
		SSL_load_error_strings();
	}

	void WebFramework::HttpsData::setPathToCertificate(const std::filesystem::path& pathToCertificate)
	{
		this->pathToCertificate = pathToCertificate;
		this->pathToCertificate = this->pathToCertificate.make_preferred();
	}

	void WebFramework::HttpsData::setPathToKey(const std::filesystem::path& pathToKey)
	{
		this->pathToKey = pathToKey;
		this->pathToKey = this->pathToKey.make_preferred();
	}

	const std::filesystem::path& WebFramework::HttpsData::getPathToCertificate() const
	{
		return pathToCertificate;
	}

	const std::filesystem::path& WebFramework::HttpsData::getPathToKey() const
	{
		return pathToKey;
	}

	void WebFramework::parseAdditionalConfigs(const json::JsonObject& webFrameworkSettings, const std::filesystem::path& basePath, std::vector<std::string>& settingsPaths, std::vector<std::string>& loadSources)
	{
		std::vector<json::JsonObject> additionalConfigs;

		if (!webFrameworkSettings.tryGet<std::vector<json::JsonObject>>(json_settings::additionalConfigsKey, additionalConfigs))
		{
			return;
		}

		std::vector<std::string> configs = json::utility::JsonArrayWrapper(additionalConfigs).as<std::string>();

		std::ranges::for_each(configs, [&basePath](std::string& path) { path = (basePath / path).string(); });

		for (const std::string& configPath : configs)
		{
			utility::Config config(configPath);
			const json::JsonParser& parser = *config;
			std::vector<std::string> tempSettingsPaths = json::utility::JsonArrayWrapper(parser.get<std::vector<json::JsonObject>>(json_settings::settingsPathsKey)).as<std::string>();
			std::vector<std::string> tempLoadSources = json::utility::JsonArrayWrapper(parser.get<std::vector<json::JsonObject>>(json_settings::loadSourcesKey)).as<std::string>();

			std::move(tempSettingsPaths.begin(), tempSettingsPaths.end(), std::back_inserter(settingsPaths));
			std::move(tempLoadSources.begin(), tempLoadSources.end(), std::back_inserter(loadSources));
		}
	}

	uint64_t WebFramework::parseLoggingFlags(const json::JsonObject& loggingSettings) const
	{
		std::vector<json::JsonObject> flags;

		return loggingSettings.tryGet<std::vector<json::JsonObject>>(json_settings::logFlagsKey, flags) ?
			Log::createFlags(json::utility::JsonArrayWrapper(flags).as<std::string>()) :
			(std::numeric_limits<uint64_t>::max)();
	}

	Log::VerbosityLevel WebFramework::parseVerbosity(const json::JsonObject& loggingSettings) const
	{
		std::string verbosity;

		if (loggingSettings.tryGet<std::string>(json_settings::logVerbosityLevelKey, verbosity))
		{
			if (verbosity == "verbose")
			{
				return Log::VerbosityLevel::verbose;
			}
			else if (verbosity == "warning")
			{
				return Log::VerbosityLevel::warning;
			}
			else if (verbosity == "error")
			{
				return Log::VerbosityLevel::error;
			}
		}

		return Log::VerbosityLevel::verbose;
	}

	std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings> WebFramework::createExecutorsSettings(const std::vector<std::string>& settingsPaths)
	{
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings> result;

		for (const std::string& settingsPath : settingsPaths)
		{
			utility::JSONSettingsParser parser(settingsPath, *this);

			for (const auto& [key, value] : parser.getSettings())
			{
				if (auto it = result.find(key); it != result.end())
				{
					utility::logAndThrowException<logging::message::sameExecutorRoute, logging::category::webFramework>(value.name, it->second.name);
				}

				result.try_emplace(key, value);
			}
		}

		return result;
	}

	void WebFramework::initAPIs(const json::JsonObject& webFrameworkSettings)
	{
		const std::unordered_map<std::string_view, std::function<void()>> initFunctions =
		{
#ifdef __WITH_PYTHON_EXECUTORS__
			{
				json_settings_values::runtimesPythonValue,
				[]() { runtime::RuntimesManager::get().addRuntime<runtime::PythonRuntime>(); }
			},
#endif
#ifdef __WITH_DOTNET_EXECUTORS__
			{
				json_settings_values::runtimesDotNetValue,
				[]() { runtime::RuntimesManager::get().addRuntime<runtime::DotNetRuntime>(); }
			}
#endif
		};
		std::vector<json::JsonObject> runtimes;

		webFrameworkSettings.tryGet<std::vector<json::JsonObject>>(json_settings::runtimesKey, runtimes);

		runtime::RuntimesManager::get().addRuntime<runtime::CXXRuntime>();
		runtime::RuntimesManager::get().addRuntime<runtime::CCRuntime>();

		for (const json::JsonObject& runtimeObject : runtimes)
		{
			const std::string* runtime = nullptr;

			if (runtimeObject.is<std::string>())
			{
				runtime = &runtimeObject.get<std::string>();
			}
			else if (runtimeObject.is<json::JsonObject>())
			{
				bool enabled = false;

				if (runtimeObject.tryGet<bool>("enabled", enabled); !enabled)
				{
					continue;
				}

				runtime = &runtimeObject["name"].get<std::string>();
			}
			else
			{
				utility::logAndThrowException<logging::message::wrongRuntimeObjectType, logging::category::runtime>();
			}

			if (auto it = initFunctions.find(*runtime); it == initFunctions.end())
			{
				utility::logAndThrowException<logging::message::wrongRuntime, logging::category::runtime>(*runtime);
			}
			else
			{
				if (Log::isValid())
				{
					Log::info<logging::message::addRuntime, logging::category::runtime>(*runtime);
				}

				it->second();
			}
		}
	}

	void WebFramework::initLogging() const
	{
		json::JsonObject loggingSettings;

		if (!(*config).tryGet<json::JsonObject>(json_settings::loggingObject, loggingSettings))
		{
			return;
		}

		bool usingLogging = false;

		if (loggingSettings.tryGet<bool>(json_settings::usingLoggingKey, usingLogging) && usingLogging)
		{
			std::string logsPath;
			const std::string& dateFormat = loggingSettings[json_settings::dateFormatKey].get<std::string>();
			bool duplicateOutput = false;
			bool duplicateErrorOutput = false;

			loggingSettings.tryGet<std::string>(json_settings::logsPathKey, logsPath);

			loggingSettings.tryGet<bool>(json_settings::duplicateOutputKey, duplicateOutput);
			loggingSettings.tryGet<bool>(json_settings::duplicateErrorOutputKey, duplicateErrorOutput);

			uint64_t logFileSize = 0;
			uint64_t flags = this->parseLoggingFlags(loggingSettings);
			Log::VerbosityLevel verbosity = this->parseVerbosity(loggingSettings);

			if (loggingSettings.tryGet<uint64_t>(json_settings::logFileSizeKey, logFileSize))
			{
				if (flags == (std::numeric_limits<uint64_t>::max)())
				{
					Log::configure(dateFormat, logsPath, logFileSize);
				}
				else
				{
					Log::configure(dateFormat, logsPath, logFileSize, flags);
				}
			}
			else
			{
				if (flags == (std::numeric_limits<uint64_t>::max)())
				{
					Log::configure(dateFormat, logsPath);
				}
				else
				{
					Log::configure(dateFormat, logsPath, Log::logFileSize, flags);
				}
			}

			if (duplicateOutput)
			{
				Log::duplicateLog(std::cout);
			}

			if (duplicateErrorOutput)
			{
				Log::duplicateErrorLog(std::cerr);
			}

			Log::setVerbosityLevel(verbosity);
		}
	}

	void WebFramework::initExecutors(const json::JsonObject& webFrameworkSettings, std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>& executorSettings, std::vector<std::string>& pathToSources)
	{
		const std::filesystem::path& basePath = config.getBasePath();
		std::vector<std::string> settingsPaths;

		{
			std::vector<json::JsonObject> settings;

			if (webFrameworkSettings.tryGet(json_settings::settingsPathsKey, settings))
			{
				settingsPaths = json::utility::JsonArrayWrapper(settings).as<std::string>();
			}
		}

		{
			std::vector<json::JsonObject> loadSources;

			if (webFrameworkSettings.tryGet(json_settings::loadSourcesKey, loadSources))
			{
				pathToSources = json::utility::JsonArrayWrapper(loadSources).as<std::string>();
			}
		}

		WebFramework::parseAdditionalConfigs(webFrameworkSettings, basePath, settingsPaths, pathToSources);

		std::ranges::for_each(settingsPaths, [this, &basePath](std::string& path) { path = (basePath / path).make_preferred().string(); });
		std::ranges::for_each
		(
			pathToSources, [this, &basePath](std::string& source)
			{
				if (source == json_settings_values::defaultLoadSourceValue)
				{
					return;
				}

				source = (basePath / source).make_preferred().string();
			}
		);

		executorSettings = this->createExecutorsSettings(settingsPaths);

		if (ExecutorsManager::types.at(webFrameworkSettings[json_settings::webServerTypeKey].get<std::string>()) > ExecutorsManager::WebServerType::proxy)
		{
			if (settingsPaths.empty())
			{
				utility::logAndThrowException<logging::message::cantFindKey, logging::category::webFramework>(json_settings::settingsPathsKey);
			}

			if (pathToSources.empty())
			{
				utility::logAndThrowException<logging::message::cantFindKey, logging::category::webFramework>(json_settings::loadSourcesKey);
			}
		}
	}

	void WebFramework::initTaskExecutors(const json::JsonObject& taskBrokerObject)
	{
		std::vector<json::JsonObject> taskExecutorPaths;
		std::vector<utility::TaskExecutorsSettings> taskExecutorsSettings;

		taskBrokerObject.tryGet<std::vector<json::JsonObject>>(json_settings::taskExecutorsSettingsKey, taskExecutorPaths);

		const std::filesystem::path& basePath = config.getBasePath();
		task_broker::TaskExecutorsManager& taskExecutorsManager = task_broker::TaskExecutorsManager::get();
		std::string consumer;

		if (taskBrokerObject.tryGet<std::string>(json_settings::consumerKey, consumer) && consumer == json_settings_values::consumerInternalValue)
		{
			if (taskExecutorPaths.empty())
			{
				utility::logAndThrowException<logging::message::cantFindTaskExecutorPaths, logging::category::webFramework>(json_settings_values::consumerInternalValue, json_settings::taskExecutorsSettingsKey);
			}

			task_broker::TaskBrokersManager& taskBrokerManager = task_broker::TaskBrokersManager::get();
			const std::vector<json::JsonObject>& taskBrokers = taskBrokerObject.at(json_settings::taskBrokersKey).get<std::vector<json::JsonObject>>();
			std::vector<std::string> taskBrokerNames;
			size_t consumerThreads = json_settings_values::consumerThreadsDefaultValue;
			size_t checkPeriod = json_settings_values::checkPeriodDefaultValue;

			taskBrokerObject.tryGet<size_t>(json_settings::consumerThreadsKey, consumerThreads);
			taskBrokerObject.tryGet<size_t>(json_settings::checkPeriodKey, checkPeriod);

			json::JsonObject emptySettings;

			for (const json::JsonObject& taskBroker : taskBrokers)
			{
				const std::string* taskBrokerName = nullptr;
				const json::JsonObject* settings = nullptr;

				if (taskBroker.is<std::string>())
				{
					taskBrokerName = &taskBroker.get<std::string>();
					settings = &emptySettings;
				}
				else if (taskBroker.is<json::JsonObject>())
				{
					taskBrokerName = &taskBroker["name"].get<std::string>();
					settings = &taskBroker["settings"];
				}
				else
				{
					utility::logAndThrowException<logging::message::cantParseTaskBrokerArray, logging::category::webFramework>(taskBroker.getType().name());
				}

				taskBrokerManager.addTaskBroker(*taskBrokerName, *settings);

				taskBrokerNames.emplace_back(*taskBrokerName);
			}

			taskExecutorsManager.createTaskConsumer(taskBrokerNames, consumerThreads, std::chrono::milliseconds(checkPeriod));
		}

		for (const json::JsonObject& taskExecutorPath : taskExecutorPaths)
		{
			std::filesystem::path temp = taskExecutorPath.get<std::string>();
			std::ifstream stream;

			if (temp.is_absolute())
			{
				stream.open(temp);
			}
			else
			{
				stream.open(basePath / temp);
			}

			json::JsonParser parser(stream);

			for (const json::JsonObject& settings : parser.getParsedData().get<std::vector<json::JsonObject>>())
			{
				taskExecutorsSettings.emplace_back(utility::TaskExecutorsSettings::createTaskExecutorsSettings(settings));
			}

			taskExecutorsManager.initTaskExecutor(taskExecutorsSettings);
		}

		taskExecutorsManager.runTaskConsumer(); // run only if consumer created
	}

	void WebFramework::initHTTPS(const json::JsonObject& webFrameworkSettings)
	{
		json::JsonObject https;

		if (!webFrameworkSettings.tryGet<json::JsonObject>(json_settings::httpsObject, https))
		{
			return;
		}

		if (bool useHTTPS = false; https.tryGet<bool>(json_settings::useHTTPSKey, useHTTPS) && useHTTPS)
		{
			HttpsData& data = httpsData.emplace();
			const std::filesystem::path& basePath = config.getBasePath();

			data.setPathToCertificate(basePath / https[json_settings::pathToCertificateKey].get<std::string>());
			data.setPathToKey(basePath / https[json_settings::pathToKey].get<std::string>());

			if (Log::isValid())
			{
				Log::info<logging::message::httpsInitialization, logging::category::https>(data.getPathToCertificate().string(), data.getPathToKey().string());
			}
		}
	}

	void WebFramework::initDatabase(const json::JsonObject& webFrameworkSettings)
	{
		std::vector<std::string> databases;

		if (std::vector<json::JsonObject> temp; webFrameworkSettings.tryGet<std::vector<json::JsonObject>>(json_settings::databasesKey, temp))
		{
			databases = json::utility::JsonArrayWrapper(temp).as<std::string>();
		}
		else if (std::string database; webFrameworkSettings.tryGet<std::string>(json_settings::databasesKey, database))
		{
			databases.emplace_back(std::move(database));
		}

		if (databases.empty())
		{
			databases.emplace_back(database::implementation::sqlite);
		}

		if (Log::isValid())
		{
			for (const std::string& database : databases)
			{
				Log::info<logging::message::databaseInitialization, logging::category::database>(database);
			}
		}

		databasesManager.initDatabaseImplementation(databases);
	}

	void WebFramework::initServer
	(
		const json::JsonObject& webFrameworkSettings,
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
		const std::vector<std::string>& pathToSources
	)
	{
		const std::filesystem::path& basePath = config.getBasePath();
		const json::JsonObject& webServerSettings = (*config).get<json::JsonObject>(json_settings::webServerObject);
		const std::string& ip = webServerSettings[json_settings::ipKey].get<std::string>();
		std::string port = std::to_string(webServerSettings[json_settings::portKey].get<int64_t>());
		DWORD timeout = 0;
		utility::AdditionalServerSettings additionalSettings = utility::AdditionalServerSettings::createSettings(webFrameworkSettings, basePath);
		int64_t resourcesThreads = 1;

		webServerType = webFrameworkSettings[json_settings::webServerTypeKey].get<std::string>();

		{
			int64_t temp = 0;

			if (webServerSettings.tryGet<int64_t>(json_settings::timeoutKey, temp))
			{
				timeout = static_cast<DWORD>(temp);
			}
		}

		webFrameworkSettings.tryGet<int64_t>(json_settings::resourcesThreadsKey, resourcesThreads);

		std::shared_ptr<threading::ThreadPool> threadPool = std::make_shared<threading::ThreadPool>(resourcesThreads);

		if (webServerType == json_settings_values::multiThreadedWebServerTypeValue)
		{
			server = std::make_unique<MultithreadedWebServer>
				(
					*config,
					std::move(executorsSettings),
					ip,
					port,
					timeout,
					pathToSources,
					additionalSettings,
					threadPool,
					*this
				);
		}
		else if (webServerType == json_settings_values::threadPoolWebServerTypeValue)
		{
			json::JsonObject threadPoolServerObject;
			uint32_t threadPoolThreads = 1;

			if ((*config).tryGet<json::JsonObject>(json_settings::threadPoolServerObject, threadPoolServerObject))
			{
				(*config).tryGet<uint32_t>(json_settings::threadCountKey, threadPoolThreads);
			}

			server = std::make_unique<ThreadPoolWebServer>
				(
					*config,
					std::move(executorsSettings),
					ip,
					port,
					timeout,
					pathToSources,
					additionalSettings,
					threadPoolThreads,
					threadPool,
					*this
				);
		}
		else if (webServerType == json_settings_values::loadBalancerWebServerTypeValue)
		{
			const json::JsonObject& loadBalancerSettings = (*config).get<json::JsonObject>(json_settings::loadBalancerObject);
			json::JsonObject heuristic;
			std::string loadSource(json_settings_values::defaultLoadSourceValue);
			bool serversHTTPS = loadBalancerSettings[json_settings::serversHTTPSKey].get<bool>();
			const json::JsonObject& listOfServers = loadBalancerSettings[json_settings::listOfServersKey].get<json::JsonObject>();
			json::MapJsonIterator iterator(listOfServers);
			std::unordered_map<std::string, std::vector<int64_t>> allServers;
			uint64_t processingThreads = 1;
			uint64_t targetRPS = json_settings_values::targetRPSValue;

			loadBalancerSettings.tryGet<std::string>(json_settings::loadSourceKey, loadSource);
			loadBalancerSettings.tryGet<uint64_t>(json_settings::processingThreadsKey, processingThreads);
			loadBalancerSettings.tryGet<uint64_t>(json_settings::targetRPSKey, targetRPS);

			if (!loadBalancerSettings.tryGet<json::JsonObject>(json_settings::heuristicKey, heuristic))
			{
				heuristic[json_settings::heuristicNameKey] = json_settings_values::defaultHeuristicValue;
				heuristic[json_settings::apiTypeKey] = json_settings::cxxExecutorKey;
			}

			for (const auto& [key, value] : iterator)
			{
				allServers.try_emplace
				(
					key,
					json::utility::JsonArrayWrapper(value).as<int64_t>()
				);
			}

			server = std::make_unique<load_balancer::LoadBalancerServer>
				(
					ip,
					port,
					timeout,
					serversHTTPS,
					heuristic,
					utility::loadSources({ loadSource }).front().first,
					allServers,
					std::make_shared<ResourceExecutor>(*config, additionalSettings, threadPool),
					static_cast<uint32_t>(processingThreads),
					static_cast<uint32_t>(targetRPS),
					*this
				);
		}
		else if (webServerType == json_settings_values::proxyWebServerTypeValue)
		{
			server = std::make_unique<proxy::ProxyServer>(ip, port, timeout, (*config).get<json::JsonObject>(json_settings::proxyObject), *this);
		}
		else
		{
			utility::logAndThrowException<logging::message::wrongWebServerType, logging::category::webFramework>();
		}
	}

	WebFramework::WebFramework(const utility::Config& webFrameworkConfig) :
		config(webFrameworkConfig),
		serverException(nullptr)
	{
		this->initLogging();

		const json::JsonObject& webFrameworkSettings = (*config).get<json::JsonObject>(json_settings::webFrameworkObject);
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings> executorsSettings;
		runtime::RuntimesManager& runtimesManager = runtime::RuntimesManager::get();
		std::vector<std::string> pathToSources;

		this->initAPIs(webFrameworkSettings);
		this->initDatabase(webFrameworkSettings);
		this->initExecutors(webFrameworkSettings, executorsSettings, pathToSources);
		this->initHTTPS(webFrameworkSettings);
		this->initServer(webFrameworkSettings, std::move(executorsSettings), pathToSources);

		if (json::JsonObject taskBrokerObject; (*config).tryGet<json::JsonObject>(json_settings::taskBrokerObject, taskBrokerObject))
		{
			this->initTaskExecutors(taskBrokerObject);
		}

		for (auto it = runtimesManager.begin(); it != runtimesManager.end(); ++it)
		{
			it->finishInitialization();
		}

		if (!utility::isVariableExist("JWT_SECRET"))
		{
			std::mt19937 random(static_cast<uint32_t>(std::time(nullptr)));

			utility::setEnvironmentVariable("JWT_SECRET", utility::generateRandomString(random() % 64));
		}
	}

	WebFramework::WebFramework(const std::filesystem::path& webFrameworkConfigPath) :
		WebFramework(utility::Config(webFrameworkConfigPath))
	{

	}

	void WebFramework::start(bool wait, const std::function<void()>& onStartServer)
	{
		if (Log::isValid())
		{
			Log::info<logging::message::startingServer, logging::category::webFrameworkServer>(webServerType, server->getIp(), server->getPort());
		}

		server->start(wait, onStartServer, serverException);
	}

	void WebFramework::stop(bool wait)
	{
		if (Log::isValid())
		{
			Log::info<logging::message::stoppingServer, logging::category::webFrameworkServer>(webServerType, server->getIp(), server->getPort());
		}

		server->stop(wait);
	}

	void WebFramework::kick(const std::string& ip) const
	{
		if (Log::isValid())
		{
			Log::info<logging::message::kickClient, logging::category::webFrameworkServer>(ip);
		}

		server->kick(ip);
	}

	void WebFramework::updateSslCertificates()
	{
		server->updateCertificates();
	}

	bool WebFramework::isServerRunning() const
	{
		return server->isServerRunning();
	}

	std::vector<std::string> WebFramework::getClientsIp() const
	{
		std::vector<std::pair<std::string, std::vector<SOCKET>>> clients = server->getClients();
		std::vector<std::string> result;

		result.reserve(clients.size());

		for (const auto& [ip, _] : clients)
		{
			result.push_back(ip);
		}

		return result;
	}

	const json::JsonParser& WebFramework::getCurrentConfiguration() const
	{
		return (*config);
	}

	const std::optional<WebFramework::HttpsData>& WebFramework::getHttpsData() const
	{
		return httpsData;
	}

	DatabasesManager& WebFramework::getDatabasesManager()
	{
		return databasesManager;
	}

	WebFramework::~WebFramework()
	{
		if (serverException)
		{
			if (Log::isValid())
			{
				Log::error<logging::message::serverException, logging::category::webFrameworkServer>((*serverException)->what());
			}

			delete (*serverException);
		}
	}
}
