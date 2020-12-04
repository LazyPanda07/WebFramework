#include "WebFramework.h"

#include "INIParser.h"
#include "WebFrameworkConstants.h"
#include "Exceptions/FileDoesNotExistException.h"

#pragma comment (lib, "BaseTCPServer.lib")
#pragma comment (lib, "HTTP.lib")
#pragma comment (lib, "Log.lib")
#pragma comment (lib, "SocketStreams.lib")
#pragma comment (lib, "INIParser.lib")

using namespace std;

namespace framework
{
	WebFramework::WebFramework(const filesystem::path& configurationINIFile)
	{
		if (!filesystem::exists(configurationINIFile))
		{
			throw exceptions::FileDoesNotExistException();
		}

		::utility::INIParser parser(configurationINIFile);
		const unordered_map<string, string>& webServerSettings = parser.getSection(ini::webServerSection);
		const unordered_map<string, string>& webFrameworkSettigns = parser.getSection(ini::webFrameworkSection);

		server = make_unique<WebServer>(utility::XMLSettingsParser(webFrameworkSettigns.at(ini::settingsPathKey)), webServerSettings.at(ini::portKey), stoi(webServerSettings.at(ini::timeoutKey)));
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
