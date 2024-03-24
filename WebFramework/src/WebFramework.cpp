#include "WebFramework.h"

#include "Exceptions/BaseJSONException.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "WebNetwork/WebServers/MultithreadedWebServer.h"
#include "WebNetwork/WebServers/ThreadPoolWebServer.h"
#include "Utility/Singletons/HTTPSSingleton.h"

using namespace std;

namespace framework
{
	string WebFramework::getWebFrameworkVersion()
	{
		return "2.6.0"s;
	}

	WebFramework::WebFramework(const filesystem::path& configurationJSONFile) :
		configurationJSONFile(configurationJSONFile),
		basePath(configurationJSONFile)
	{
		if (!filesystem::exists(configurationJSONFile))
		{
			throw file_manager::exceptions::FileDoesNotExistException(configurationJSONFile.string());
		}

		currentConfiguration.setJSONData((ifstream(configurationJSONFile)));
		basePath.remove_filename();

		const json::utility::jsonObject& webServerSettings = currentConfiguration.getObject(json_settings::webServerObject);
		const string& ip = webServerSettings.getString(json_settings::ipKey);
		const string& port = webServerSettings.getString(json_settings::portKey);
		DWORD timeout = static_cast<DWORD>(webServerSettings.getInt(json_settings::timeoutKey));

		const json::utility::jsonObject& webFrameworkSettings = currentConfiguration.getObject(json_settings::webFrameworkObject);
		vector<string> settingsPaths = json::utility::JSONArrayWrapper(webFrameworkSettings.getArray(json_settings::settingsPathsKey)).getAsStringArray();
		vector<string> loadSources = json::utility::JSONArrayWrapper(webFrameworkSettings.getArray(json_settings::loadSourcesKey)).getAsStringArray();
		string assetsPath = (basePath / webFrameworkSettings.getString(json_settings::assetsPathKey)).string();
		string templatesPath = (basePath / webFrameworkSettings.getString(json_settings::templatesPathKey)).string();
		uint64_t cachingSize = webFrameworkSettings.getUnsignedInt(json_settings::cachingSize);
		const string& webServerType = webFrameworkSettings.getString(json_settings::webServerTypeKey);

		ranges::for_each(settingsPaths, [this](string& path) {path = (basePath / path).string(); });
		ranges::for_each(loadSources, [this](string& source)
			{
				if (source == "current")
				{
					return;
				}

				source = (basePath / source).string();
			});

		try
		{
			const json::utility::jsonObject& loggingSettings = currentConfiguration.getObject(json_settings::loggingObject); // is logging object exists

			try
			{
				const string& dateFormat = loggingSettings.getString(json_settings::dateFormatKey);

				if (loggingSettings.getBool(json_settings::usingLoggingKey))
				{
					try
					{
						Log::configure(dateFormat, basePath, loggingSettings.getUnsignedInt(json_settings::logFileSizeKey));
					}
					catch (const json::exceptions::BaseJSONException&)
					{
						Log::configure(dateFormat, basePath);
					}
				}
			}
			catch (const json::exceptions::BaseJSONException& e)
			{
				cout << e.what() << endl;

				return;
			}
		}
		catch (const json::exceptions::BaseJSONException&)
		{

		}

		try
		{
			if (webFrameworkSettings.getBool(json_settings::useHTTPSKey))
			{
				utility::HTTPSSingleton& httpsSettings = utility::HTTPSSingleton::get();

				httpsSettings.setUseHTTPS(true);
				httpsSettings.setPathToCertificate(basePath / webFrameworkSettings.getString(json_settings::pathToCertificateKey));
				httpsSettings.setPathToKey(basePath / webFrameworkSettings.getString(json_settings::pathToKey));

				SSL_library_init();
				SSL_load_error_strings();
			}
		}
		catch (const json::exceptions::BaseJSONException&)
		{

		}

		vector<utility::JSONSettingsParser> jsonSettings;

		jsonSettings.reserve(settingsPaths.size());

		transform(settingsPaths.begin(), settingsPaths.end(), back_inserter(jsonSettings), [](const string& i) { return utility::JSONSettingsParser(i); });

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
		else
		{
			throw out_of_range(::exceptions::wrongWebServerType);
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
