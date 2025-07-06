#pragma once

#include "Framework/WebFrameworkPlatform.h"

#include "Executors/BaseExecutor.h"
#include "Utility/JSONSettingsParser.h"
#include "Executors/ResourceExecutor.h"
#include "Utility/RouteParameters.h"
#include "Utility/AdditionalServerSettings.h"
#include "Framework/WebFrameworkConstants.h"
#include <Utility/Sources.h>

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

		class StatefulExecutors
		{
		private:
			std::unordered_map<std::string, std::unique_ptr<BaseExecutor>> statefulExecutors;

		public:
			StatefulExecutors() = default;

			std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& operator *();

			~StatefulExecutors();
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
		std::unordered_map<std::string, HMODULE> creatorSources; // executor name - shared library
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings> settings; // route - executor settings
		std::shared_ptr<ResourceExecutor> resources;
		std::vector<utility::RouteParameters> routeParameters; // base routes for parameterize executors
		std::string userAgentFilter;
		WebServerType serverType;

	private:
		static bool isFileRequest(std::string_view parameters);

		static bool isHeavyOperation(BaseExecutor* executor);

		static void parseRouteParameters(const std::string& parameters, HTTPRequest& request, std::vector<utility::RouteParameters>::iterator it);

	private:
		BaseExecutor* getOrCreateExecutor(std::string& parameters, HTTPRequest& request, std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& statefulExecutors);

		bool filterUserAgent(const std::string& parameters, const web::HeadersMap& headers, HTTPResponse& response) const;

		std::unique_ptr<BaseExecutor> createAPIExecutor(const std::string& name, std::string_view apiType) const;

	public:
		ExecutorsManager
		(
			const json::JSONParser& configuration,
			const std::vector<std::string>& pathToSources,
			std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& executorsSettings,
			const utility::AdditionalServerSettings& additionalSettings,
			threading::ThreadPool& threadPool
		);

		ExecutorsManager(const ExecutorsManager&) = delete;

		ExecutorsManager& operator = (const ExecutorsManager&) = delete;

		ExecutorsManager(ExecutorsManager&& other) noexcept;

		ExecutorsManager& operator = (ExecutorsManager&& other) noexcept;

		std::optional<std::function<void(HTTPRequest&, HTTPResponse&)>> service(HTTPRequest& request, HTTPResponse& response, std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& statefulExecutors);

		BaseExecutor* getOrCreateExecutor(HTTPRequest& request, HTTPResponse& response, std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& statefulExecutors);

		std::shared_ptr<ResourceExecutor> getResourceExecutor() const;

		~ExecutorsManager() = default;
	};
}
