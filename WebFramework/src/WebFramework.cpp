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
		const unordered_multimap<string, string>& webServerSettings = parser.getSection(ini::webServerSection);
		const unordered_multimap<string, string>& webFrameworkSettings = parser.getSection(ini::webFrameworkSection);

		try
		{
			auto settingsPath = webFrameworkSettings.equal_range(ini::settingsPathKey);
			auto assetsPath = webFrameworkSettings.equal_range(ini::assetsPathKey);
			auto usingAssetsCache = webFrameworkSettings.equal_range(ini::usingAssetsCacheKey);
			auto loadSource = webFrameworkSettings.equal_range(ini::loadSourceKey);
			auto port = webServerSettings.equal_range(ini::portKey);
			auto timeout = webServerSettings.equal_range(ini::timeoutKey);

			if (settingsPath.first == webFrameworkSettings.end())
			{
				throw out_of_range(::exceptions::cantFindSettingsPath);
			}

			if (assetsPath.first == webFrameworkSettings.end())
			{
				throw out_of_range(::exceptions::cantFindAssetsPath);
			}

			if (usingAssetsCache.first == webFrameworkSettings.end())
			{
				throw out_of_range(::exceptions::cantFindUsingAssetsCache);
			}

			if (loadSource.first == webFrameworkSettings.end())
			{
				throw out_of_range(::exceptions::cantFindLoadSource);
			}

			if (port.first == webServerSettings.end())
			{
				throw out_of_range(::exceptions::cantFindPort);
			}

			if (timeout.first == webServerSettings.end())
			{
				throw out_of_range(::exceptions::timeout);
			}

			server = make_unique<WebServer>
				(
					utility::XMLSettingsParser(settingsPath.first->second),
					assetsPath.first->second,
					usingAssetsCache.first->second == "true" ? true : false,
					port.first->second,
					stoi(timeout.first->second),
					loadSource.first->second
					);
		}
		catch (const exceptions::BaseExecutorException&)
		{
			throw;
		}
		catch (const out_of_range&)	//not found settings in unordered_multimap
		{
			throw;
		}
		catch (const invalid_argument&)	//stoi
		{
			throw;
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
