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
		return "2.2"s;
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

		vector<string> settingsPaths = json::utility::JSONArrayWrapper(currentConfiguration.getArray(json_settings::settingsPathsKey)).getAsStringArray();
		vector<string> loadSources = json::utility::JSONArrayWrapper(currentConfiguration.getArray(json_settings::loadSourcesKey)).getAsStringArray();
		string assetsPath = (basePath / currentConfiguration.getString(json_settings::assetsPathKey)).string();
		string templatesPath = (basePath / currentConfiguration.getString(json_settings::templatesPathKey)).string();
		uint64_t cachingSize = currentConfiguration.getUnsignedInt(json_settings::cachingSize);
		const string& webServerType = currentConfiguration.getString(json_settings::webServerTypeKey);
		const string& ip = currentConfiguration.getString(json_settings::ipKey);
		const string& port = currentConfiguration.getString(json_settings::portKey);
		DWORD timeout = static_cast<DWORD>(currentConfiguration.getInt(json_settings::timeoutKey));
		bool useHTTPS = false;

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
			currentConfiguration.getObject(json_settings::loggingObject);	// is logging object exists

			try
			{
				const string& dateFormat = currentConfiguration.getString(json_settings::dateFormatKey);

				if (currentConfiguration.getBool(json_settings::usingLoggingKey))
				{
					try
					{
						Log::init(Log::dateFormatFromString(dateFormat), currentConfiguration.getBool(json_settings::addNewLineAfterLogKey), basePath);
					}
					catch (const json::exceptions::BaseJSONException&)
					{
						Log::init(Log::dateFormatFromString(dateFormat), true, basePath);
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
			useHTTPS = currentConfiguration.getBool(json_settings::useHTTPSKey);

			if (useHTTPS)
			{
				utility::HTTPSSingleton& httpsSettings = utility::HTTPSSingleton::get();

				httpsSettings.setUseHTTPS(true);
				httpsSettings.setPathToCertificate(currentConfiguration.getString(json_settings::pathToCertificateKey));
				httpsSettings.setPathToKey(currentConfiguration.getString(json_settings::pathToKey));

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
						static_cast<uint32_t>(currentConfiguration.getInt("threadCount"))
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

	void WebFramework::startServer()
	{
		server->start();
	}

	void WebFramework::stopServer()
	{
		server->stop();
	}

	void WebFramework::disconnectClient(const string& ip) const
	{
		server->pubDisconnect(ip);
	}

	vector<string> WebFramework::getClientsIp() const
	{
		const vector<pair<string, SOCKET>> clients = server->getClients();
		vector<string> result;

		result.reserve(clients.size());

		for (const auto& [ip, _] : clients)
		{
			result.push_back(ip);
		}

		return result;
	}

	bool WebFramework::getServerState() const
	{
		return server->serverState();
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
