#include "WebFramework.h"

#include "INIParser.h"
#include "WebFrameworkConstants.h"
#include "Exceptions/FileDoesNotExistException.h"
#include "Log.h"

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
			throw exceptions::FileDoesNotExistException(configurationINIFile.string());
		}

		::utility::INIParser parser(configurationINIFile);
		const unordered_multimap<string, string>& webServerSettings = parser.getSection(ini::webServerSection);
		const unordered_multimap<string, string>& webFrameworkSettings = parser.getSection(ini::webFrameworkSection);
		const unordered_multimap<string, string>& loggingSettings = parser.getSection(ini::loggingSection);

		try
		{
			auto settingsPath = webFrameworkSettings.equal_range(ini::settingsPathKey);
			auto assetsPath = webFrameworkSettings.equal_range(ini::assetsPathKey);
			auto templatesPath = webFrameworkSettings.equal_range(ini::templatesPathKey);
			auto usingAssetsCache = webFrameworkSettings.equal_range(ini::usingAssetsCacheKey);
			auto loadSourcesIterator = webFrameworkSettings.equal_range(ini::loadSourceKey);
			auto ip = webServerSettings.equal_range(ini::ipKey);
			auto port = webServerSettings.equal_range(ini::portKey);
			auto timeout = webServerSettings.equal_range(ini::timeoutKey);
			auto usingLogging = loggingSettings.equal_range(ini::usingLoggingKey);
			auto dateFormat = loggingSettings.equal_range(ini::dateFormatKey);
			auto addNewLineAfterLog = loggingSettings.equal_range(ini::addNewLineAfterLogKey);

			if (settingsPath.first == webFrameworkSettings.end())
			{
				throw out_of_range(::exceptions::cantFindSettingsPath);
			}

			if (assetsPath.first == webFrameworkSettings.end())
			{
				throw out_of_range(::exceptions::cantFindAssetsPath);
			}

			if (templatesPath.first == webFrameworkSettings.end())
			{
				throw out_of_range(::exceptions::cantFindTemplatesPath);
			}

			if (usingAssetsCache.first == webFrameworkSettings.end())
			{
				throw out_of_range(::exceptions::cantFindUsingAssetsCache);
			}

			if (loadSourcesIterator.first == webFrameworkSettings.end())
			{
				throw out_of_range(::exceptions::cantFindLoadSource);
			}

			if (ip.first == webServerSettings.end())
			{
				throw out_of_range(::exceptions::cantFindIp);
			}

			if (port.first == webServerSettings.end())
			{
				throw out_of_range(::exceptions::cantFindPort);
			}

			if (timeout.first == webServerSettings.end())
			{
				throw out_of_range(::exceptions::cantFindTimeout);
			}

			if (usingLogging.first != loggingSettings.end())
			{
				if (usingLogging.first->second == "true")
				{
					if (dateFormat.first == loggingSettings.end())
					{
						throw out_of_range(::exceptions::cantFindDateFormat);
					}

					if (addNewLineAfterLog.first == loggingSettings.end())
					{
						Log::init(Log::dateFormatFromString(dateFormat.first->second));
					}
					else
					{
						Log::init(Log::dateFormatFromString(dateFormat.first->second), addNewLineAfterLog.first->second == "true" ? true : false);
					}
				}
			}

			vector<string> loadSources;

			transform(loadSourcesIterator.first, loadSourcesIterator.second, back_inserter(loadSources), [](const auto& i) { return i.second; });	//take values of loadSource array

			server = make_unique<WebServer>
				(
					utility::XMLSettingsParser(settingsPath.first->second),
					assetsPath.first->second,
					templatesPath.first->second,
					usingAssetsCache.first->second == "true" ? true : false,
					ip.first->second,
					port.first->second,
					stoi(timeout.first->second),
					loadSources
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
		catch (const invalid_argument&)	//stoi or wrong dateFormat
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
