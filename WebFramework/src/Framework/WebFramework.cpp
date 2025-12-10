#include "WebFramework.h"

#include <filesystem>

#include <Log.h>
#include <JsonArrayWrapper.h>
#include <MapJsonIterator.h>

#include "Exceptions/BaseJSONException.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "Web/Servers/MultithreadedWebServer.h"
#include "Web/Servers/ThreadPoolWebServer.h"
#include "LoadBalancer/LoadBalancerServer.h"
#include "Utility/Singletons/HTTPSSingleton.h"
#include "Utility/Sources.h"
#include "Proxy/ProxyServer.h"
#include "Utility/DynamicLibraries.h"
#include "Framework/WebFrameworkConstants.h"
#include "Managers/DatabasesManager.h"
#include "Managers/RuntimesManager.h"
#include "Runtimes/CXXRuntime.h"
#include "Runtimes/CCRuntime.h"
#include "Runtimes/PythonRuntime.h"
#include "Runtimes/DotNetRuntime.h"

namespace framework
{
	bool WebFramework::getUseHTTPS()
	{
		return utility::HTTPSSingleton::get().getUseHTTPS();
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

			std::move(tempSettingsPaths.begin(), tempSettingsPaths.end(), back_inserter(settingsPaths));
			std::move(tempLoadSources.begin(), tempLoadSources.end(), back_inserter(loadSources));
		}
	}

	std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings> WebFramework::createExecutorsSettings(const std::vector<std::string>& settingsPaths)
	{
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings> result;

		for (const std::string& settingsPath : settingsPaths)
		{
			utility::JSONSettingsParser parser(settingsPath);

			for (const auto& [key, value] : parser.getSettings())
			{
				if (auto it = result.find(key); it != result.end())
				{
					throw std::runtime_error(std::format("Executor {} has same route as {}", value.name, it->second.name));
				}

				result.try_emplace(key, value);
			}
		}

		return result;
	}

	uint64_t WebFramework::parseLoggingFlags(const json::JsonObject& loggingSettings) const
	{
		std::vector<json::JsonObject> flags;

		return loggingSettings.tryGet<std::vector<json::JsonObject>>(json_settings::logFlagsKey, flags) ?
			Log::createFlags(json::utility::JsonArrayWrapper(flags).as<std::string>()) :
			(std::numeric_limits<uint64_t>::max)();
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
#ifdef __WITH_DOT_NET_EXECUTORS__
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
				throw std::runtime_error("Wrong runtimes value type");
			}

			if (auto it = initFunctions.find(*runtime); it == initFunctions.end())
			{
				std::string message = std::format("Can't load {} runtime", *runtime);

				if (Log::isValid())
				{
					Log::fatalError(message, "LogRuntime", 15);
				}

				throw std::runtime_error(message);
			}
			else
			{
				if (Log::isValid())
				{
					Log::info("Add {} runtime", "LogRuntime", *runtime);
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

		std::ranges::for_each(settingsPaths, [this, &basePath](std::string& path) { path = (basePath / path).string(); });
		std::ranges::for_each
		(
			pathToSources, [this, &basePath](std::string& source)
			{
				if (source == json_settings_values::defaultLoadSourceValue)
				{
					return;
				}

				source = (basePath / source).string();
			}
		);

		executorSettings = WebFramework::createExecutorsSettings(settingsPaths);

		if (ExecutorsManager::types.at(webFrameworkSettings[json_settings::webServerTypeKey].get<std::string>()) > ExecutorsManager::WebServerType::proxy)
		{
			if (settingsPaths.empty())
			{
				throw std::runtime_error(format("Can't find {}", json_settings::settingsPathsKey));
			}

			if (pathToSources.empty())
			{
				throw std::runtime_error(format("Can't find {}", json_settings::loadSourcesKey));
			}
		}
	}

	void WebFramework::initHTTPS(const json::JsonObject& webFrameworkSettings) const
	{
		json::JsonObject https;

		if (!webFrameworkSettings.tryGet<json::JsonObject>(json_settings::httpsObject, https))
		{
			return;
		}

		bool useHTTPS = false;

		if (https.tryGet<bool>(json_settings::useHTTPSKey, useHTTPS) && useHTTPS)
		{
			utility::HTTPSSingleton& httpsSettings = utility::HTTPSSingleton::get();
			const std::filesystem::path& basePath = config.getBasePath();

			httpsSettings.setUseHTTPS(true);
			httpsSettings.setPathToCertificate(basePath / https[json_settings::pathToCertificateKey].get<std::string>());
			httpsSettings.setPathToKey(basePath / https[json_settings::pathToKey].get<std::string>());

			SSL_library_init();
			SSL_load_error_strings();

			if (Log::isValid())
			{
				Log::info("Using HTTPS with certificate: {}, key: {}", "LogWebFramework", httpsSettings.getPathToCertificate().string(), httpsSettings.getPathToKey().string());
			}
		}
	}

	void WebFramework::initDatabase(const json::JsonObject& webFrameworkSettings)
	{
		std::string databaseImplementationName;

		if (!webFrameworkSettings.tryGet<std::string>(json_settings::databaseImplementationKey, databaseImplementationName))
		{
			databaseImplementationName = "sqlite";
		}

		if (Log::isValid())
		{
			Log::info("Using {} database", "LogWebFramework", databaseImplementationName);
		}

		DatabasesManager::get().initDatabaseImplementation(databaseImplementationName);
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
			server = make_unique<MultithreadedWebServer>
				(
					*config,
					move(executorsSettings),
					ip,
					port,
					timeout,
					pathToSources,
					additionalSettings,
					threadPool
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

			server = make_unique<ThreadPoolWebServer>
				(
					*config,
					move(executorsSettings),
					ip,
					port,
					timeout,
					pathToSources,
					additionalSettings,
					threadPoolThreads,
					threadPool
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
				heuristic["name"] = json_settings_values::defaultHeuristicValue;
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
					make_shared<ResourceExecutor>(*config, additionalSettings, threadPool),
					static_cast<uint32_t>(processingThreads),
					static_cast<uint32_t>(targetRPS)
				);
		}
		else if (webServerType == json_settings_values::proxyWebServerTypeValue)
		{
			server = std::make_unique<proxy::ProxyServer>(ip, port, timeout, (*config).get<json::JsonObject>(json_settings::proxyObject));
		}
		else
		{
			throw std::runtime_error(::exceptions::wrongWebServerType);
		}
	}

	WebFramework::WebFramework(const utility::Config& webFrameworkConfig) :
		config(webFrameworkConfig),
		serverException(nullptr)
	{
		this->initLogging();

		const json::JsonObject& webFrameworkSettings = (*config).get<json::JsonObject>(json_settings::webFrameworkObject);
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings> executorsSettings;
		runtime::RuntimesManager& instance = runtime::RuntimesManager::get();
		std::vector<std::string> pathToSources;

		this->initAPIs(webFrameworkSettings);

		this->initDatabase(webFrameworkSettings);
		this->initExecutors(webFrameworkSettings, executorsSettings, pathToSources);
		this->initHTTPS(webFrameworkSettings);
		this->initServer
		(
			webFrameworkSettings,
			std::move(executorsSettings),
			pathToSources
		);

		for (auto it = instance.begin(); it != instance.end(); ++it)
		{
			it->finishInitialization();
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
			Log::info("Starting {} server at {}:{}", "LogWebFramework", webServerType, server->getIp(), server->getPort());
		}

		server->start(wait, onStartServer, serverException);
	}

	void WebFramework::stop(bool wait)
	{
		if (Log::isValid())
		{
			Log::info("Stopping {} server at {}:{}", "LogWebFramework", webServerType, server->getIp(), server->getPort());
		}

		server->stop(wait);
	}

	void WebFramework::kick(const std::string& ip) const
	{
		if (Log::isValid())
		{
			Log::info("Kick client with ip: {} from server", "LogWebFramework", ip);
		}

		server->kick(ip);
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

	bool WebFramework::isServerRunning() const
	{
		return server->isServerRunning();
	}

	const json::JsonParser& WebFramework::getCurrentConfiguration() const
	{
		return (*config);
	}

	WebFramework::~WebFramework()
	{
		if (serverException)
		{
			if (Log::isValid())
			{
				Log::error("Server exception: {}", "LogWebFrameworkServer", (*serverException)->what());
			}

			delete (*serverException);
		}
	}
}
