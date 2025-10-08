#pragma once

#include "Framework/WebFrameworkPlatform.h"

#include "Executors/BaseExecutor.h"
#include "Utility/JSONSettingsParser.h"
#include "Executors/ResourceExecutor.h"
#include "Utility/RouteParameters.h"
#include "Utility/AdditionalServerSettings.h"
#include "Framework/WebFrameworkConstants.h"
#include "Utility/Sources.h"

namespace framework
{
	class ExecutorsManager
	{
	public:
		enum class WebServerType
		{
			loadBalancer,
			proxy,
			multiThreaded,
			threadPool
		};
		
		static inline const std::unordered_map<std::string_view, WebServerType> types =
		{
			{ json_settings::loadBalancerWebServerTypeValue, WebServerType::loadBalancer },
			{ json_settings::proxyWebServerTypeValue, WebServerType::proxy },
			{ json_settings::multiThreadedWebServerTypeValue, WebServerType::multiThreaded },
			{ json_settings::threadPoolWebServerTypeValue, WebServerType::threadPool }
		};

	private:
		std::mutex checkExecutor;
		std::unordered_map<std::string, std::unique_ptr<BaseExecutor>> routes; // route - executor
		std::unordered_map<std::string, CreateExecutorFunction> creators; // executor name - create function
		std::unordered_map<std::string, utility::LoadSource> creatorSources; // executor name - shared library
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings> settings; // route - executor settings
		std::shared_ptr<ResourceExecutor> resources;
		std::vector<utility::RouteParameters> routeParameters; // base routes for parameterize executors
		std::string userAgentFilter;
		WebServerType serverType;

	private:
		static bool isFileRequest(std::string_view parameters);

		static bool isHeavyOperation(BaseExecutor* executor);

		static void parseRouteParameters(const std::string& parameters, HTTPRequestExecutors& request, std::vector<utility::RouteParameters>::iterator it);

		static void callInitFunction(const utility::LoadSource& creatorSource, std::string_view webFrameworkSharedLibraryPath, std::string_view apiType);

	private:
		BaseExecutor* getOrCreateExecutor(std::string& parameters, HTTPRequestExecutors& request, std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& statefulExecutors);

		bool filterUserAgent(const std::string& parameters, const web::HeadersMap& headers, HTTPResponseExecutors& response) const;

		std::unique_ptr<BaseExecutor> createAPIExecutor(const std::string& name, std::string_view apiType) const;

		void initCreators(const std::vector<std::string>& pathToSources);

	public:
		ExecutorsManager
		(
			const json::JSONParser& configuration,
			const std::vector<std::string>& pathToSources,
			std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
			const utility::AdditionalServerSettings& additionalSettings,
			std::shared_ptr<threading::ThreadPool> threadPool
		);

		ExecutorsManager(const ExecutorsManager&) = delete;

		ExecutorsManager& operator = (const ExecutorsManager&) = delete;

		ExecutorsManager(ExecutorsManager&& other) noexcept;

		ExecutorsManager& operator = (ExecutorsManager&& other) noexcept;

		std::optional<std::function<void(HTTPRequestExecutors&, HTTPResponseExecutors&)>> service(HTTPRequestExecutors& request, HTTPResponseExecutors& response, std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& statefulExecutors);

		BaseExecutor* getOrCreateExecutor(HTTPRequestExecutors& request, HTTPResponseExecutors& response, std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& statefulExecutors);

		std::shared_ptr<ResourceExecutor> getResourceExecutor() const;

		~ExecutorsManager() = default;
	};
}
