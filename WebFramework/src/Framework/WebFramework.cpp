#include "WebFramework.h"

#include <filesystem>

#ifdef __WITH_PYTHON_EXECUTORS__
#include <pybind11/embed.h>
#endif

#include <Log.h>
#include <JSONArrayWrapper.h>

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

namespace framework
{
	bool WebFramework::getUseHTTPS()
	{
		return utility::HTTPSSingleton::get().getUseHTTPS();
	}

	void WebFramework::parseAdditionalConfigs(const json::utility::jsonObject& webFrameworkSettings, const std::filesystem::path& basePath, std::vector<std::string>& settingsPaths, std::vector<std::string>& loadSources)
	{
		std::vector<json::utility::jsonObject> additionalConfigs;

		if (!webFrameworkSettings.tryGetArray(json_settings::additionalConfigsKey, additionalConfigs))
		{
			return;
		}

		std::vector<std::string> configs = json::utility::JSONArrayWrapper(additionalConfigs).getAsStringArray();

		std::ranges::for_each(configs, [&basePath](std::string& path) { path = (basePath / path).string(); });

		for (const std::string& configPath : configs)
		{
			utility::Config config(configPath);
			const json::JSONParser& parser = *config;
			std::vector<std::string> tempSettingsPaths = json::utility::JSONArrayWrapper(parser.getArray(json_settings::settingsPathsKey)).getAsStringArray();
			std::vector<std::string> tempLoadSources = json::utility::JSONArrayWrapper(parser.getArray(json_settings::loadSourcesKey)).getAsStringArray();

			move(tempSettingsPaths.begin(), tempSettingsPaths.end(), back_inserter(settingsPaths));
			move(tempLoadSources.begin(), tempLoadSources.end(), back_inserter(loadSources));
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
				result.try_emplace(key, value);
			}
		}

		return result;
	}

	uint64_t WebFramework::parseLoggingFlags(const json::utility::jsonObject& loggingSettings) const
	{
		std::vector<json::utility::jsonObject> flags;

		return loggingSettings.tryGetArray(json_settings::logFlagsKey, flags) ?
			Log::createFlags(json::utility::JSONArrayWrapper(flags).getAsStringArray()) :
			(std::numeric_limits<uint64_t>::max)();
	}

	void WebFramework::initAPIs() const
	{
#ifdef __WITH_PYTHON_EXECUTORS__
		pybind11::initialize_interpreter();
#endif
	}

	void WebFramework::initLogging() const
	{
		json::utility::jsonObject loggingSettings;

		if (!(*config).tryGetObject(json_settings::loggingObject, loggingSettings))
		{
			return;
		}

		bool usingLogging = false;

		if (loggingSettings.tryGetBool(json_settings::usingLoggingKey, usingLogging) && usingLogging)
		{
			std::string logsPath;
			const std::string& dateFormat = loggingSettings.getString(json_settings::dateFormatKey);
			bool duplicateOutput = false;
			bool duplicateErrorOutput = false;

			loggingSettings.tryGetString(json_settings::logsPathKey, logsPath);

			loggingSettings.tryGetBool(json_settings::duplicateOutputKey, duplicateOutput);
			loggingSettings.tryGetBool(json_settings::duplicateErrorOutputKey, duplicateErrorOutput);

			uint64_t logFileSize = 0;
			uint64_t flags = this->parseLoggingFlags(loggingSettings);

			if (loggingSettings.tryGetUnsignedInt(json_settings::logFileSizeKey, logFileSize))
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

	void WebFramework::initExecutors(const json::utility::jsonObject& webFrameworkSettings, std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>& executorSettings, std::vector<std::string>& pathToSources)
	{
		const std::filesystem::path& basePath = config.getBasePath();
		std::vector<std::string> settingsPaths;

		{
			std::vector<json::utility::jsonObject> settings;

			if (webFrameworkSettings.tryGetArray(json_settings::settingsPathsKey, settings))
			{
				settingsPaths = json::utility::JSONArrayWrapper(settings).getAsStringArray();
			}
		}

		{
			std::vector<json::utility::jsonObject> loadSources;

			if (webFrameworkSettings.tryGetArray(json_settings::loadSourcesKey, loadSources))
			{
				pathToSources = json::utility::JSONArrayWrapper(loadSources).getAsStringArray();
			}
		}

		WebFramework::parseAdditionalConfigs(webFrameworkSettings, basePath, settingsPaths, pathToSources);

		std::ranges::for_each(settingsPaths, [this, &basePath](std::string& path) { path = (basePath / path).string(); });
		std::ranges::for_each
		(
			pathToSources, [this, &basePath](std::string& source)
			{
				if (source == json_settings::defaultLoadSourceValue)
				{
					return;
				}

				source = (basePath / source).string();
			}
		);

		executorSettings = WebFramework::createExecutorsSettings(settingsPaths);

		if (ExecutorsManager::types.at(webFrameworkSettings.getString(json_settings::webServerTypeKey)) > ExecutorsManager::WebServerType::proxy)
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

	void WebFramework::initHTTPS(const json::utility::jsonObject& webFrameworkSettings) const
	{
		json::utility::jsonObject https;

		if (!webFrameworkSettings.tryGetObject(json_settings::httpsObject, https))
		{
			return;
		}

		bool useHTTPS = false;

		if (https.tryGetBool(json_settings::useHTTPSKey, useHTTPS) && useHTTPS)
		{
			utility::HTTPSSingleton& httpsSettings = utility::HTTPSSingleton::get();
			const std::filesystem::path& basePath = config.getBasePath();

			httpsSettings.setUseHTTPS(true);
			httpsSettings.setPathToCertificate(basePath / https.getString(json_settings::pathToCertificateKey));
			httpsSettings.setPathToKey(basePath / https.getString(json_settings::pathToKey));

			SSL_library_init();
			SSL_load_error_strings();

			if (Log::isValid())
			{
				Log::info("Using HTTPS with certificate: {}, key: {}", "LogWebFramework", httpsSettings.getPathToCertificate().string(), httpsSettings.getPathToKey().string());
			}
		}
	}

	void WebFramework::initDatabase(const json::utility::jsonObject& webFrameworkSettings)
	{
		std::string databaseImplementationName;

		if (!webFrameworkSettings.tryGetString(json_settings::databaseImplementationKey, databaseImplementationName))
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
		const json::utility::jsonObject& webFrameworkSettings,
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
		const std::vector<std::string>& pathToSources
	)
	{
		const std::filesystem::path& basePath = config.getBasePath();
		const json::utility::jsonObject& webServerSettings = (*config).getObject(json_settings::webServerObject);
		const std::string& ip = webServerSettings.getString(json_settings::ipKey);
		std::string port = std::to_string(webServerSettings.getInt(json_settings::portKey));
		DWORD timeout = 0;
		utility::AdditionalServerSettings additionalSettings = utility::AdditionalServerSettings::createSettings(webFrameworkSettings, basePath);
		int64_t resourcesThreads = 1;

		webServerType = webFrameworkSettings.getString(json_settings::webServerTypeKey);

		{
			int64_t temp = 0;

			if (webServerSettings.tryGetInt(json_settings::timeoutKey, temp))
			{
				timeout = static_cast<DWORD>(temp);
			}
		}

		webFrameworkSettings.tryGetInt(json_settings::resourcesThreadsKey, resourcesThreads);

		std::shared_ptr<threading::ThreadPool> threadPool = std::make_shared<threading::ThreadPool>(resourcesThreads);

		if (webServerType == json_settings::multiThreadedWebServerTypeValue)
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
		else if (webServerType == json_settings::threadPoolWebServerTypeValue)
		{
			json::utility::jsonObject threadPoolServerObject;
			uint64_t threadPoolThreads = 1;

			if ((*config).tryGetObject(json_settings::threadPoolServerObject, threadPoolServerObject))
			{
				(*config).tryGetUnsignedInt(json_settings::threadCountKey, threadPoolThreads);
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
		else if (webServerType == json_settings::loadBalancerWebServerTypeValue)
		{
			const json::utility::jsonObject& loadBalancerSettings = (*config).getObject(json_settings::loadBalancerObject);
			json::utility::jsonObject heuristic;
			std::string loadSource(json_settings::defaultLoadSourceValue);
			bool serversHTTPS = loadBalancerSettings.getBool(json_settings::serversHTTPSKey);
			const json::utility::jsonObject& listOfServers = loadBalancerSettings.getObject(json_settings::listOfServersKey);
			std::unordered_map<std::string, std::vector<int64_t>> allServers;
			uint64_t processingThreads = 1;

			loadBalancerSettings.tryGetString(json_settings::loadSourceKey, loadSource);
			loadBalancerSettings.tryGetUnsignedInt(json_settings::processingThreadsKey, processingThreads);

			if (!loadBalancerSettings.tryGetObject(json_settings::heuristicKey, heuristic))
			{
				heuristic.setString("name", json_settings::defaultHeuristicValue);
				heuristic.setString(json_settings::apiTypeKey, json_settings::cxxExecutorKey);
			}

			for (const auto& [key, value] : listOfServers)
			{
				allServers.try_emplace
				(
					key,
					json::utility::JSONArrayWrapper(std::get<std::vector<json::utility::jsonObject>>(value)).getAsInt64_tArray()
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
					processingThreads
				);
		}
		else if (webServerType == json_settings::proxyWebServerTypeValue)
		{
			server = std::make_unique<proxy::ProxyServer>(ip, port, timeout, (*config).getObject(json_settings::proxyObject));
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
		this->initAPIs();

		this->initLogging();

		const json::utility::jsonObject& webFrameworkSettings = (*config).getObject(json_settings::webFrameworkObject);
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings> executorsSettings;
		std::vector<std::string> pathToSources;

		this->initDatabase(webFrameworkSettings);
		this->initExecutors(webFrameworkSettings, executorsSettings, pathToSources);
		this->initHTTPS(webFrameworkSettings);
		this->initServer
		(
			webFrameworkSettings,
			std::move(executorsSettings),
			pathToSources
		);
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

	const json::JSONParser& WebFramework::getCurrentConfiguration() const
	{
		return (*config);
	}

	WebFramework::~WebFramework()
	{
#ifdef __WITH_PYTHON_EXECUTORS__
		pybind11::finalize_interpreter();
#endif

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
