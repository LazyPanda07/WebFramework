#pragma once

#include "headers.h"

#include "WebFrameworkManagers/ExecutorsManager.h"
#include "WebFrameworkManagers/SessionsManager.h"
#include "SQLite3/SQLiteManager.h"
#include "Utility/JSONSettingsParser.h"

namespace framework
{
	class BaseWebServer : public virtual web::BaseTCPServer
	{
	protected:
		ExecutorsManager executorsManager;
		SessionsManager sessionsManager;
		sqlite::SQLiteManager databasesManager;
		std::weak_ptr<ResourceExecutor> resources;

	public:
		/// <summary>
		/// Construct web server
		/// </summary>
		/// <param name="configuration">settings from main .json file</param>
		/// <param name="parsers">settings from .json files</param>
		/// <param name="assets">path to assets folder</param>
		/// <param name="pathToTemplates">path to templates folder</param>
		/// <param name="cachingSize">size of available cache in bytes</param>
		/// <param name="ip">0.0.0.0 alias for running in all ips</param>
		/// <param name="port">default is 80 or 443</param>
		/// <param name="timeout">wait for client connection in milliseconds</param>
		/// <param name="pathToSources">paths to files with executors</param>
		/// <exception cref="file_manager::exceptions::FileDoesNotExistException"></exception>
		/// <exception cref="framework::exceptions::CantLoadSourceException"></exception>
		/// <exception cref="framework::exceptions::CantFindFunctionException"></exception>
		/// <exception cref="framework::exceptions::MissingLoadTypeException"></exception>
		BaseWebServer(const json::JSONParser& configuration, const std::vector<utility::JSONSettingsParser>& parsers, const std::filesystem::path& assets, const std::string& pathToTemplates, uint64_t cachingSize, const std::string& ip, const std::string& port, DWORD timeout, const std::vector<std::string>& pathToSources);

		virtual ~BaseWebServer() = default;
	};
}
