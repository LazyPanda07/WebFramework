#pragma once

#include "core.h"

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
		sqlite::SQLiteManager databaseManager;
		std::shared_ptr<ResourceExecutor> resources;
		SSL_CTX* context;
		bool useHTTPS;

	public:
		BaseWebServer(const json::JSONParser& configuration, const std::vector<utility::JSONSettingsParser>& parsers, const std::filesystem::path& assets, const std::string& pathToTemplates, uint64_t cachingSize, const std::string& ip, const std::string& port, DWORD timeout, const std::vector<std::string>& pathToSources);

		virtual ~BaseWebServer();
	};
}
