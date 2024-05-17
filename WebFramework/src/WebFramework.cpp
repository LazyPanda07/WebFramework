#include "WebFramework.h"

#include "Log.h"
#include "JSONArrayWrapper.h"

#include "Exceptions/BaseJSONException.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "WebNetwork/WebServers/MultithreadedWebServer.h"
#include "WebNetwork/WebServers/ThreadPoolWebServer.h"
#include "LoadBalancer/LoadBalancerServer.h"
#include "Utility/Singletons/HTTPSSingleton.h"

using namespace std;

namespace framework
{
	string WebFramework::getWebFrameworkVersion()
	{
		string version = "2.6.0";

		return version;
	}

	bool WebFramework::getUseHTTPS()
	{
		return utility::HTTPSSingleton::get().getUseHTTPS();
	}

	string WebFramework::initLogging() const
	{
		try
		{
			const json::utility::jsonObject& loggingSettings = currentConfiguration.getObject(json_settings::loggingObject); // is logging object exists

			try
			{
				if (loggingSettings.getBool(json_settings::usingLoggingKey))
				{
					string logsPath;
					const string& dateFormat = loggingSettings.getString(json_settings::dateFormatKey);

					loggingSettings.tryGetString(json_settings::logsPathKey, logsPath);

					try
					{
						Log::configure(dateFormat, logsPath, loggingSettings.getUnsignedInt(json_settings::logFileSizeKey));
					}
					catch (const json::exceptions::BaseJSONException&)
					{
						Log::configure(dateFormat, logsPath);
					}
				}
			}
			catch (const json::exceptions::BaseJSONException& e)
			{
				return e.what();
			}
		}
		catch (const json::exceptions::BaseJSONException& e)
		{
			cerr << e.what() << endl;
		}

		return "";
	}

	void WebFramework::initHTTPS(const json::utility::jsonObject& webFrameworkSettings) const
	{
		try
		{
			const json::utility::jsonObject& https = webFrameworkSettings.getObject(json_settings::httpsObject);

			if (https.getBool(json_settings::useHTTPSKey))
			{
				utility::HTTPSSingleton& httpsSettings = utility::HTTPSSingleton::get();

				httpsSettings.setUseHTTPS(true);
				httpsSettings.setPathToCertificate(basePath / https.getString(json_settings::pathToCertificateKey));
				httpsSettings.setPathToKey(basePath / https.getString(json_settings::pathToKey));

				SSL_library_init();
				SSL_load_error_strings();
			}
		}
		catch (const json::exceptions::BaseJSONException& e)
		{
			cerr << e.what() << endl;
		}
	}

	void WebFramework::initServer
	(
		const json::utility::jsonObject& webFrameworkSettings,
		const vector<utility::JSONSettingsParser>& jsonSettings,
		const vector<string>& loadSources
	)
	{
		const json::utility::jsonObject& webServerSettings = currentConfiguration.getObject(json_settings::webServerObject);
		const string& ip = webServerSettings.getString(json_settings::ipKey);
		const string& port = webServerSettings.getString(json_settings::portKey);
		DWORD timeout = static_cast<DWORD>(webServerSettings.getInt(json_settings::timeoutKey));

		const string& webServerType = webFrameworkSettings.getString(json_settings::webServerTypeKey);
		string assetsPath = (basePath / webFrameworkSettings.getString(json_settings::assetsPathKey)).string();
		string templatesPath = (basePath / webFrameworkSettings.getString(json_settings::templatesPathKey)).string();
		uint64_t cachingSize = webFrameworkSettings.getUnsignedInt(json_settings::cachingSize);

		if (webServerType == json_settings::multiThreadedWebServerTypeValue)
		{
			server = make_unique<MultithreadedWebServer>
				(
					currentConfiguration,
					jsonSettings,
					assetsPath,
					templatesPath,
					cachingSize,
					ip,
					port,
					timeout,
					loadSources
				);
		}
		else if (webServerType == json_settings::threadPoolWebServerTypeValue)
		{
			try
			{
				server = make_unique<ThreadPoolWebServer>
					(
						currentConfiguration,
						jsonSettings,
						assetsPath,
						templatesPath,
						cachingSize,
						ip,
						port,
						timeout,
						loadSources,
						static_cast<uint32_t>(currentConfiguration.getObject(json_settings::threadPoolServerObject).getInt("threadCount"))
					);
			}
			catch (const json::exceptions::BaseJSONException&)
			{
				server = make_unique<ThreadPoolWebServer>
					(
						currentConfiguration,
						jsonSettings,
						assetsPath,
						templatesPath,
						cachingSize,
						ip,
						port,
						timeout,
						loadSources,
						NULL
					);
			}
		}
		else if (webServerType == json_settings::loadBalancerWebServerTypeValue)
		{
			const json::utility::jsonObject& loadBalancerSettings = currentConfiguration.getObject(json_settings::loadBalancerObject);
			const json::utility::jsonObject& listOfServers = loadBalancerSettings.getObject("listOfServers");
			unordered_map<string, vector<string>> allServers;

			for (const auto& [key, value] : listOfServers)
			{
				allServers.emplace
				(
					key,
					json::utility::JSONArrayWrapper(get<vector<json::utility::jsonObject>>(value)).getAsStringArray()
				);
			}

			server = make_unique<load_balancer::LoadBalancerServer>(ip, port, timeout, allServers);
		}
		else if (webServerType == json_settings::proxyWebServerTypeValue)
		{

		}
	}

	WebFramework::WebFramework(const filesystem::path& configurationJSONFile) :
		configurationJSONFile(configurationJSONFile),
		basePath(filesystem::absolute(configurationJSONFile))
	{
		if (!filesystem::exists(configurationJSONFile))
		{
			throw file_manager::exceptions::FileDoesNotExistException(configurationJSONFile.string());
		}

		currentConfiguration.setJSONData((ifstream(configurationJSONFile)));

		basePath.remove_filename();

		const json::utility::jsonObject& webFrameworkSettings = currentConfiguration.getObject(json_settings::webFrameworkObject);
		vector<string> settingsPaths = json::utility::JSONArrayWrapper(webFrameworkSettings.getArray(json_settings::settingsPathsKey)).getAsStringArray();
		vector<string> loadSources = json::utility::JSONArrayWrapper(webFrameworkSettings.getArray(json_settings::loadSourcesKey)).getAsStringArray();

		ranges::for_each(settingsPaths, [this](string& path) { path = (basePath / path).string(); });
		ranges::for_each(loadSources, [this](string& source)
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
			loadSources
		);

		if (!server)
		{
			throw runtime_error(::exceptions::wrongWebServerType);
		}
	}

	void WebFramework::startServer(bool wait, const function<void()>& onStartServer)
	{
		server->start(wait, onStartServer);
	}

	void WebFramework::stopServer(bool wait)
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
		return currentConfiguration;
	}

	const filesystem::path& WebFramework::getConfigurationJSONFile() const
	{
		return configurationJSONFile;
	}
}
