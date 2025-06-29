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

	public:
		IExecutorFunctionality
		(
			const json::JSONParser& configuration,
			const std::filesystem::path& assets,
			const std::filesystem::path& pathToTemplates,
			uint64_t cachingSize,
			std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
			const std::vector<std::string>& pathToSources,
			const utility::AdditionalServerSettings& additionalSettings
		);

		virtual ~IExecutorFunctionality() = default;
	};
}
