#pragma once

#include "BaseTCPServer.h"
#include "WebFrameworkConstants.h"
#include "WebFrameworkManagers/ExecutorsManager.h"
#include "WebFrameworkManagers/SessionsManager.h"
#include "SQLite3/SQLiteManager.h"
#include "Utility/JSONSettingsParser.h"

namespace framework
{
	/// <summary>
	/// Standard TCP web server
	/// <para>Initialize only through WebFramework</para>
	/// </summary>
	class MultithreadingWebServer : public web::BaseTCPServer
	{
	private:
		ExecutorsManager executorsManager;
		SessionsManager sessionsManager;
		sqlite::SQLiteManager databasesManager;

	private:
		/// <summary>
		/// Called for every client
		/// </summary>
		/// <param name="clientSocket">client's socket</param>
		/// <param name="addr">client's address</param>
		void clientConnection(SOCKET clientSocket, sockaddr addr) override;

	public:
		/// <summary>
		/// Construct MultithreadingWebServer
		/// </summary>
		/// <param name="settings">settings from .json files</param>
		/// <param name="assets">path to assets folder</param>
		/// <param name="pathToTemplates">path to templates folder</param>
		/// <param name="isCaching">is caching resource files</param>
		/// <param name="ip">0.0.0.0 alias for running in all ips</param>
		/// <param name="port">default is 80 or 443</param>
		/// <param name="timeout">wait for client connection in milliseconds</param>
		/// <param name="pathToSources">paths to files with executors</param>
		/// <exception cref="framework::exceptions::FileDoesNotExistException"></exception>
		/// <exception cref="framework::exceptions::CantLoadSourceException"></exception>
		/// <exception cref="framework::exceptions::CantFindFunctionException"></exception>
		/// <exception cref="framework::exceptions::MissingLoadTypeException"></exception>
		MultithreadingWebServer(const std::vector<utility::JSONSettingsParser>& parsers, const std::filesystem::path& assets, const std::string& pathToTemplates, bool isCaching, const std::string& ip, const std::string& port, DWORD timeout, const std::vector<std::string>& pathToSources);

		~MultithreadingWebServer() = default;
	};
}
