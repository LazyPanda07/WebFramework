#pragma once

#include "Framework/WebFrameworkPlatform.h"

#include <MultiLocalizationManager.h>
#include <IOSocketStream.h>

#include "Managers/ExecutorsManager.h"
#include "Managers/SessionsManager.h"
#include "Utility/AdditionalServerSettings.h"
#include "Utility/LargeFileHandlers/BaseLargeBodyHandler.h"
#include "Web/HTTPRequestImplementation.h"

namespace framework
{
	class ExecutorServer
	{
	protected:
		enum class ServiceState
		{
			success,
			skipResponse,
			error
		};

	protected:
		utility::AdditionalServerSettings additionalSettings;
		std::unique_ptr<ExecutorsManager> executorsManager;
		SessionsManager sessionsManager;
		std::shared_ptr<ResourceExecutor> resources;

	protected:
		ServiceState serviceRequests(streams::IOSocketStream& stream, HTTPRequestImplementation& request, HTTPResponseImplementation& response, const std::function<void(ServiceState&)>& task);

	public:
		ExecutorServer
		(
			const json::JSONParser& configuration,
			std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
			const std::vector<std::string>& pathToSources,
			const utility::AdditionalServerSettings& additionalSettings,
			std::shared_ptr<threading::ThreadPool> threadPool
		);

		virtual ~ExecutorServer() = default;
	};
}
