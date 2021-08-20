#include "WebFramework.h"

#include <fstream>

#include "JSONParser.h"
#include "Exceptions/BaseJSONException.h"
#include "WebFrameworkConstants.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "Log.h"
#include "WebNetwork/WebServers/MultithreadedWebServer.h"
#include "WebNetwork/WebServers/ThreadPoolWebServer.h"
#include "Utility/Singletons/HTTPSSingleton.h"

#pragma comment (lib, "BaseTCPServer.lib")
#pragma comment (lib, "Log.lib")
#pragma comment (lib, "INIParser.lib")
#pragma comment (lib, "Networks.lib")

using namespace std;

namespace framework
{
	WebFramework::WebFramework(const filesystem::path& configurationJSONFile)
	{
		if (!filesystem::exists(configurationJSONFile))
		{
			throw exceptions::FileDoesNotExistException(configurationJSONFile.string());
		}

		json::JSONParser parser(move(ifstream(configurationJSONFile)));

		const vector<json::utility::objectSmartPointer<json::utility::jsonObject>>& settingsPathsJSON = parser.getArray(json_settings::settingsPathsKey);
		const vector<json::utility::objectSmartPointer<json::utility::jsonObject>>& loadSourcesJSON = parser.getArray(json_settings::loadSourcesKey);
		vector<string> settingsPaths;
		vector<string> loadSources;

		settingsPaths.reserve(settingsPathsJSON.size());

		loadSources.reserve(loadSourcesJSON.size());

		for (const auto& i : settingsPathsJSON)
		{
			settingsPaths.push_back(get<string>(i->data.front().second));
		}

		for (const auto& i : loadSourcesJSON)
		{
			loadSources.push_back(get<string>(i->data.front().second));
		}

		const string& assetsPath = parser.get<string>(json_settings::assetsPathKey);
		const string& templatesPath = parser.get<string>(json_settings::templatesPathKey);
		bool usingAssetsCache = parser.get<bool>(json_settings::usingAssetsCacheKey);
		const string& webServerType = parser.get<string>(json_settings::webServerTypeKey);
		const string& ip = parser.get<string>(json_settings::ipKey);
		const string& port = parser.get<string>(json_settings::portKey);
		int64_t timeout = parser.get<int64_t>(json_settings::timeoutKey);
		bool useHTTPS = false;

		try
		{
			parser.getObject(json_settings::loggingObject);	// is logging object exists

			try
			{
				bool usingLogging = parser.get<bool>(json_settings::usingLoggingKey);
				const string& dateFormat = parser.get<string>(json_settings::dateFormatKey);

				if (usingLogging)
				{
					try
					{
						bool addNewLineAfterLog = parser.get<bool>(json_settings::addNewLineAfterLogKey);

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
			useHTTPS = parser.get<bool>(json_settings::useHTTPSKey);

			if (useHTTPS)
			{
				utility::HTTPSSingleton& httpsSettings = utility::HTTPSSingleton::get();

				httpsSettings.setUseHTTPS(true);
				httpsSettings.setPathToCertificate(parser.get<string>(json_settings::pathToCertificateKey));
				httpsSettings.setPathToKey(parser.get<string>(json_settings::pathToKey));

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

		if (webServerType == json_settings::multithreadedWebServerTypeValue)
		{
			server = make_unique<MultithreadedWebServer>
				(
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
						jsonSettings,
						assetsPath,
						templatesPath,
						usingAssetsCache,
						ip,
						port,
						timeout,
						loadSources,
						static_cast<uint32_t>(parser.get<int64_t>("threadCount"))
						);
			}
			catch (const json::exceptions::BaseJSONException&)
			{
				server = make_unique<ThreadPoolWebServer>
					(
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

	vector<string> WebFramework::getClientsIp() const
	{
		const vector<pair<string, SOCKET>> clients = server->getClients();
		vector<string> result(clients.size());

		for (size_t i = 0; i < clients.size(); i++)
		{
			result[i] = clients[i].first;
		}

		return result;
	}

	bool WebFramework::getServerState() const
	{
		return server->serverState();
	}

	void WebFramework::disconnectClient(const string& ip) const
	{
		server->pubDisconnect(ip);
	}
}
