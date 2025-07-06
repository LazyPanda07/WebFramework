#pragma once

#include "Framework/WebFrameworkPlatform.h"

#include "MultiLocalizationManager.h"

#include "Managers/ExecutorsManager.h"
#include "Managers/SessionsManager.h"
#include "Utility/AdditionalServerSettings.h"

namespace framework
{
	class ExecutorServer
	{
	protected:
		utility::AdditionalServerSettings additionalSettings;
		std::unique_ptr<ExecutorsManager> executorsManager;
		SessionsManager sessionsManager;
		std::shared_ptr<ResourceExecutor> resources;

	public:
		ExecutorServer
		(
			const json::JSONParser& configuration,
			std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
			const std::vector<std::string>& pathToSources,
			const utility::AdditionalServerSettings& additionalSettings,
			threading::ThreadPool& threadPool
		);

		virtual ~ExecutorServer() = default;
	};
}
