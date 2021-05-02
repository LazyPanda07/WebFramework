#include "WebFramework.h"

#include <fstream>

#include "JSONParser.h"
#include "Exceptions/BaseJSONException.h"
#include "WebFrameworkConstants.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "Log.h"
#include "WebNetwork/WebServers/MultithreadedWebServer.h"

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

		const vector<string>& settingsPaths = parser.get<vector<string>>(json::settingsPathsKey);
		const string& assetsPath = parser.get<string>(json::assetsPathKey);
		const string& templatesPath = parser.get<string>(json::templatesPathKey);
		bool usingAssetsCache = parser.get<bool>(json::usingAssetsCacheKey);
		const vector<string>& loadSources = parser.get<vector<string>>(json::loadSourcesKey);
		const string& webServerType = parser.get<string>(json::webServerTypeKey);
		const string& ip = parser.get<string>(json::ipKey);
		const string& port = parser.get<string>(json::portKey);
		uint64_t timeout = parser.get<uint64_t>(json::timeoutKey);

		try
		{
			parser.get<unique_ptr<json::JSONParser::objectType>>(json::loggingObject);	// is logging object exists

			try
			{
				bool usingLogging = parser.get<bool>(json::usingLoggingKey);
				const string& dateFormat = parser.get<string>(json::dateFormatKey);

				if (usingLogging)
				{
					try
					{
						bool addNewLineAfterLog = parser.get<bool>(json::addNewLineAfterLogKey);

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

		vector<utility::JSONSettingsParser> jsonSettings;

		transform(settingsPaths.begin(), settingsPaths.end(), back_inserter(jsonSettings), [](const string& i) { return utility::JSONSettingsParser(i); });

		if (webServerType == json::multithreadedWebServerTypeValue)
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
		else if (webServerType == json::threadPoolWebServerTypeValue)
		{

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
