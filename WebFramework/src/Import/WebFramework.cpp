#include "WebFramework.h"

#include <filesystem>

#include "Log.h"
#include "JSONArrayWrapper.h"

#include "Exceptions/BaseJSONException.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "WebNetwork/WebServers/MultithreadedWebServer.h"
#include "WebNetwork/WebServers/ThreadPoolWebServer.h"
#include "LoadBalancer/LoadBalancerServer.h"
#include "Utility/Singletons/HTTPSSingleton.h"
#include "Utility/Sources.h"
#include "Proxy/ProxyServer.h"
#include "Utility/DynamicLibraries.h"

using namespace std;

namespace framework
{
	string WebFramework::getWebFrameworkVersion()
	{
		string version = "3.0.3";

		return version;
	}

	bool WebFramework::getUseHTTPS()
	{
		return utility::HTTPSSingleton::get().getUseHTTPS();
	}

	string WebFramework::initLogging() const
	{
		json::utility::jsonObject loggingSettings;

		if (!(*config).tryGetObject(json_settings::loggingObject, loggingSettings))
		{
			return "";
		}

		bool usingLogging = false;

		if (loggingSettings.tryGetBool(json_settings::usingLoggingKey, usingLogging) && usingLogging)
		{
			string logsPath;
			const string& dateFormat = loggingSettings.getString(json_settings::dateFormatKey);
			bool duplicateOutput = false;
			bool duplicateErrorOutput = false;

			loggingSettings.tryGetString(json_settings::logsPathKey, logsPath);

			loggingSettings.tryGetBool(json_settings::duplicateOutputKey, duplicateOutput);
			loggingSettings.tryGetBool(json_settings::duplicateErrorOutputKey, duplicateErrorOutput);

			uint64_t logFileSize = 0;

			if (loggingSettings.tryGetUnsignedInt(json_settings::logFileSizeKey, logFileSize))
			{
				Log::configure(dateFormat, logsPath, logFileSize);
			}
			else
			{
				Log::configure(dateFormat, logsPath);
			}

			if (duplicateOutput)
			{
				Log::duplicateLog(cout);
			}

			if (duplicateErrorOutput)
			{
				Log::duplicateErrorLog(cerr);
			}
		}

		return "";
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
			const filesystem::path& basePath = config.getBasePath();

			httpsSettings.setUseHTTPS(true);
			httpsSettings.setPathToCertificate(basePath / https.getString(json_settings::pathToCertificateKey));
			httpsSettings.setPathToKey(basePath / https.getString(json_settings::pathToKey));

			SSL_library_init();
			SSL_load_error_strings();
		}
	}

	void WebFramework::initServer
	(
		const json::utility::jsonObject& webFrameworkSettings,
		const vector<utility::JSONSettingsParser>& jsonSettings,
		const vector<string>& pathToSources
	)
	{
		const filesystem::path& basePath = config.getBasePath();
		const json::utility::jsonObject& webServerSettings = (*config).getObject(json_settings::webServerObject);
		const string& ip = webServerSettings.getString(json_settings::ipKey);
		string port = to_string(webServerSettings.getInt(json_settings::portKey));
		DWORD timeout = static_cast<DWORD>(webServerSettings.getInt(json_settings::timeoutKey));

		const string& webServerType = webFrameworkSettings.getString(json_settings::webServerTypeKey);
		string assetsPath = (basePath / webFrameworkSettings.getString(json_settings::assetsPathKey)).string();
		string templatesPath = (basePath / webFrameworkSettings.getString(json_settings::templatesPathKey)).string();
		uint64_t cachingSize = webFrameworkSettings.getUnsignedInt(json_settings::cachingSize);

		if (webServerType == json_settings::multiThreadedWebServerTypeValue)
		{
			server = make_unique<MultithreadedWebServer>
				(
					*config,
					jsonSettings,
					assetsPath,
					templatesPath,
					cachingSize,
					ip,
					port,
					timeout,
					pathToSources
				);
		}
		else if (webServerType == json_settings::threadPoolWebServerTypeValue)
		{
			int64_t threadCount = 0;
			json::utility::jsonObject threadPoolServerObject;

			if ((*config).tryGetObject(json_settings::threadPoolServerObject, threadPoolServerObject))
			{
				(*config).tryGetInt(json_settings::threadCountKey, threadCount);
			}

			server = make_unique<ThreadPoolWebServer>
				(
					*config,
					jsonSettings,
					assetsPath,
					templatesPath,
					cachingSize,
					ip,
					port,
					timeout,
					pathToSources,
					static_cast<uint32_t>(threadCount)
				);
		}
		else if (webServerType == json_settings::loadBalancerWebServerTypeValue)
		{
			const json::utility::jsonObject& loadBalancerSettings = (*config).getObject(json_settings::loadBalancerObject);
			const string& heuristic = loadBalancerSettings.getString(json_settings::heuristicKey);
			const string& loadSource = loadBalancerSettings.getString(json_settings::loadSourceKey);
			bool serversHTTPS = loadBalancerSettings.getBool(json_settings::serversHTTPSKey);
			const json::utility::jsonObject& listOfServers = loadBalancerSettings.getObject("listOfServers");
			unordered_map<string, vector<int64_t>> allServers;

			for (const auto& [key, value] : listOfServers)
			{
				allServers.emplace
				(
					key,
					json::utility::JSONArrayWrapper(get<vector<json::utility::jsonObject>>(value)).getAsInt64_tArray()
				);
			}

			server = make_unique<load_balancer::LoadBalancerServer>
				(
					ip,
					port,
					timeout,
					serversHTTPS,
					heuristic,
					utility::loadSources(pathToSources),
					allServers,
					(*config),
					assetsPath,
					cachingSize,
					templatesPath
				);
		}
		else if (webServerType == json_settings::proxyWebServerTypeValue)
		{
			server = make_unique<proxy::ProxyServer>(ip, port, timeout, (*config).getObject(json_settings::proxyObject));
		}
	}

	WebFramework::WebFramework(const utility::Config& webFrameworkConfig) :
		config(webFrameworkConfig)
	{
		const json::utility::jsonObject& webFrameworkSettings = (*config).getObject(json_settings::webFrameworkObject);
		const filesystem::path& basePath = config.getBasePath();
		vector<string> settingsPaths = json::utility::JSONArrayWrapper(webFrameworkSettings.getArray(json_settings::settingsPathsKey)).getAsStringArray();
		vector<string> pathToSources = json::utility::JSONArrayWrapper(webFrameworkSettings.getArray(json_settings::loadSourcesKey)).getAsStringArray();

		ranges::for_each(settingsPaths, [this, &basePath](string& path) { path = (basePath / path).string(); });
		ranges::for_each(pathToSources, [this, &basePath](string& source)
			{
				if (source == "current")
				{
					return;
				}

				source = (basePath / source).string();
			});

		if (string errorMessage = this->initLogging(); errorMessage.size())
		{
			throw runtime_error(errorMessage);
		}

		this->initHTTPS(webFrameworkSettings);

		vector<utility::JSONSettingsParser> jsonSettings;

		jsonSettings.reserve(settingsPaths.size());

		transform(settingsPaths.begin(), settingsPaths.end(), back_inserter(jsonSettings), [](const string& i) { return utility::JSONSettingsParser(i); });

		this->initServer
		(
			webFrameworkSettings,
			jsonSettings,
			pathToSources
		);

		if (!server)
		{
			throw runtime_error(::exceptions::wrongWebServerType);
		}
	}

	WebFramework::WebFramework(const filesystem::path& webFrameworkConfigPath) :
		WebFramework(utility::Config(webFrameworkConfigPath))
	{

	}

	void WebFramework::start(bool wait, const function<void()>& onStartServer)
	{
		server->start(wait, onStartServer);
	}

	void WebFramework::stop(bool wait)
	{
		server->stop(wait);
	}

	void WebFramework::kick(const string& ip) const
	{
		server->kick(ip);
	}

	vector<string> WebFramework::getClientsIp() const
	{
		vector<pair<string, vector<SOCKET>>> clients = server->getClients();
		vector<string> result;

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
}
