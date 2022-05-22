#include "WebFramework.h"

#include "Exceptions/BaseJSONException.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "WebNetwork/WebServers/MultithreadedWebServer.h"
#include "WebNetwork/WebServers/ThreadPoolWebServer.h"
#include "Utility/Singletons/HTTPSSingleton.h"

#pragma comment (lib, "BaseTCPServer.lib")
#pragma comment (lib, "Log.lib")
#pragma comment (lib, "INIParser.lib")
#pragma comment (lib, "Networks.lib")
#pragma comment (lib, "Localization.lib")
#pragma comment (lib, "FileManager.lib")

using namespace std;

namespace framework
{
	string WebFramework::webFrameworkVersion()
	{
		return "2.1"s;
	}

	WebFramework::WebFramework(const filesystem::path& configurationJSONFile)
	{
		if (!filesystem::exists(configurationJSONFile))
		{
			throw file_manager::exceptions::FileDoesNotExistException(configurationJSONFile.string());
		}

		currentConfiguration = (move(ifstream(configurationJSONFile)));

		const vector<json::utility::jsonObject>& settingsPathsJSON = currentConfiguration.getArray(json_settings::settingsPathsKey);
		const vector<json::utility::jsonObject>& loadSourcesJSON = currentConfiguration.getArray(json_settings::loadSourcesKey);
		vector<string> settingsPaths;
		vector<string> loadSources;

		settingsPaths.reserve(settingsPathsJSON.size());

		loadSources.reserve(loadSourcesJSON.size());

		for (const auto& i : settingsPathsJSON)
		{
			settingsPaths.push_back(get<string>(i.data.front().second));
		}

		for (const auto& i : loadSourcesJSON)
		{
			loadSources.push_back(get<string>(i.data.front().second));
		}

		const string& assetsPath = currentConfiguration.get<string>(json_settings::assetsPathKey);
		const string& templatesPath = currentConfiguration.get<string>(json_settings::templatesPathKey);
		bool usingAssetsCache = currentConfiguration.get<bool>(json_settings::usingAssetsCacheKey);
		const string& webServerType = currentConfiguration.get<string>(json_settings::webServerTypeKey);
		const string& ip = currentConfiguration.get<string>(json_settings::ipKey);
		const string& port = currentConfiguration.get<string>(json_settings::portKey);
		DWORD timeout = static_cast<DWORD>(currentConfiguration.get<int64_t>(json_settings::timeoutKey));
		bool useHTTPS = false;

		try
		{
			currentConfiguration.getObject(json_settings::loggingObject);	// is logging object exists

			try
			{
				bool usingLogging = currentConfiguration.get<bool>(json_settings::usingLoggingKey);
				const string& dateFormat = currentConfiguration.get<string>(json_settings::dateFormatKey);

				if (usingLogging)
				{
					try
					{
						bool addNewLineAfterLog = currentConfiguration.get<bool>(json_settings::addNewLineAfterLogKey);

						Log::init(Log::dateFormatFromString(dateFormat), addNewLineAfterLog);
					}
					catch (const json::exceptions::BaseJSONException&)
					{
						Log::init(Log::dateFormatFromString(dateFormat));
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
			useHTTPS = currentConfiguration.get<bool>(json_settings::useHTTPSKey);

			if (useHTTPS)
			{
				utility::HTTPSSingleton& httpsSettings = utility::HTTPSSingleton::get();

				httpsSettings.setUseHTTPS(true);
				httpsSettings.setPathToCertificate(currentConfiguration.get<string>(json_settings::pathToCertificateKey));
				httpsSettings.setPathToKey(currentConfiguration.get<string>(json_settings::pathToKey));

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
					usingAssetsCache,
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
						usingAssetsCache,
						ip,
						port,
						timeout,
						loadSources,
						static_cast<uint32_t>(currentConfiguration.get<int64_t>("threadCount"))
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
						usingAssetsCache,
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
}
