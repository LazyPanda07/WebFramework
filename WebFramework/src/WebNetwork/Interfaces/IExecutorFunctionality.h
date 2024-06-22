#pragma once

#include "Import/WebFrameworkCore.h"

#include "MultiLocalizationManager.h"

#include "Managers/ExecutorsManager.h"
#include "Managers/SessionsManager.h"
#include "SQLite3/SQLiteManager.h"

namespace framework
{
	namespace interfaces
	{
		class IExecutorFunctionality
		{
		protected:
			ExecutorsManager executorsManager;
			SessionsManager sessionsManager;
			sqlite::SQLiteManager databaseManager;
			std::shared_ptr<ResourceExecutor> resources;

		private:
			static std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings> createExecutorSettings(const std::vector<utility::JSONSettingsParser>& parsers);

		public:
			IExecutorFunctionality
			(
				const json::JSONParser& configuration, 
				const std::filesystem::path& assets, 
				const std::filesystem::path& pathToTemplates,
				uint64_t cachingSize,
				const std::vector<utility::JSONSettingsParser>& parsers,
				const std::vector<std::string>& pathToSources
			);

			virtual ~IExecutorFunctionality() = default;
		};
	}
}
