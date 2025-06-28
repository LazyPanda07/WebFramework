#pragma once

#include "Framework/WebFrameworkPlatform.h"

#include "MultiLocalizationManager.h"

#include "Managers/ExecutorsManager.h"
#include "Managers/SessionsManager.h"
#include "Utility/AdditionalServerSettings.h"

namespace framework::interfaces
{
	class IExecutorFunctionality
	{
	protected:
		ExecutorsManager executorsManager;
		SessionsManager sessionsManager;
		std::shared_ptr<ResourceExecutor> resources;
		utility::AdditionalServerSettings additionalSettings;

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
			const std::vector<std::string>& pathToSources,
			const utility::AdditionalServerSettings& additionalSettings
		);

		virtual ~IExecutorFunctionality() = default;
	};
}
