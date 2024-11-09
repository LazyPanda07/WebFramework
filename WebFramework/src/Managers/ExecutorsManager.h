#pragma once

#include "Import/WebFrameworkCore.h"

#include "Executors/BaseExecutor.h"
#include "Utility/JSONSettingsParser.h"
#include "Executors/ResourceExecutor.h"
#include "Utility/RouteParameters.h"

namespace framework
{
	class ExecutorsManager
	{
	private:
		enum class webServerType
		{
			multiThreaded,
			threadPool,
			loadBalancer,
			proxy
		};

	private:
		std::mutex checkExecutor;
		std::unordered_map<std::string, std::unique_ptr<BaseExecutor>> routes; // route - executor
		std::unordered_map<std::string, createExecutorFunction> creators; // executor name - create function
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings> settings; // route - executor settings
		std::shared_ptr<ResourceExecutor> resources;
		std::vector<utility::RouteParameters> routeParameters; // base routes for parameterize executors
		webServerType serverType;

	private:
		static bool isFileRequest(std::string_view parameters);

		static void parseRouteParameters(const std::string& parameters, HTTPRequest& request, std::vector<utility::RouteParameters>::iterator it);

	private:
		BaseExecutor* getExecutor(std::string& parameters, HTTPRequest& request, std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& statefulExecutors);

	public:
		ExecutorsManager();

		ExecutorsManager(const ExecutorsManager&) = delete;

		ExecutorsManager& operator = (const ExecutorsManager&) = delete;

		ExecutorsManager(ExecutorsManager&& other) noexcept;

		ExecutorsManager& operator = (ExecutorsManager&& other) noexcept;

		void init
		(
			const json::JSONParser& configuraion,
			const std::filesystem::path& assets,
			uint64_t cachingSize,
			const std::filesystem::path& pathToTemplates,
			std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>&& routes,
			std::unordered_map<std::string, createExecutorFunction>&& creators,
			std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& settings,
			std::vector<utility::RouteParameters>&& routeParameters
		) noexcept;

		std::optional<std::function<void(HTTPRequest&, HTTPResponse&)>> service(HTTPRequest& request, HTTPResponse& response, std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& statefulExecutors);

		std::shared_ptr<ResourceExecutor> getResourceExecutor();

		~ExecutorsManager() = default;
	};
}
