#pragma once

#include "headers.h"

#include "Executors/BaseExecutor.h"
#include "Utility/JSONSettingsParser.h"
#include "Executors/ResourceExecutor.h"
#include "Utility/RouteParameters.h"
#include "Utility/ExecutorCreator.h"

namespace framework
{
	class ExecutorsManager
	{
	private:
		enum class webServerType
		{
			multiThreaded,
			threadPool
		};

	private:
		std::mutex checkExecutor;
		std::unordered_map<std::string, smartPointer<BaseExecutor>> routes;	// route - executor
		std::unordered_map<std::string, utility::ExecutorCreator> creators;	// executor name - create function
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings> settings;	// route - executor settings
		std::shared_ptr<ResourceExecutor> resources;
		std::vector<utility::RouteParameters> routeParameters;	// base routes for parameterize executors
		webServerType serverType;

	public:
		ExecutorsManager();

		ExecutorsManager(const ExecutorsManager&) = delete;

		ExecutorsManager& operator = (const ExecutorsManager&) = delete;

		ExecutorsManager(ExecutorsManager&& other) noexcept;

		ExecutorsManager& operator = (ExecutorsManager&& other) noexcept;

		void init(const json::JSONParser& configuraion, const std::filesystem::path& assets, uint64_t cachingSize, const std::string& pathToTemplates, std::unordered_map<std::string, smartPointer<BaseExecutor>>&& routes, std::unordered_map<std::string, utility::ExecutorCreator>&& creators, std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& settings, std::vector<utility::RouteParameters>&& routeParameters) noexcept;

		void service(HTTPRequest& request, HTTPResponse& response, std::unordered_map<std::string, smartPointer<BaseExecutor>>& statefulExecutors, std::optional<std::function<void(HTTPRequest&, HTTPResponse&)>>& threadPoolFunction);

		std::shared_ptr<ResourceExecutor> getResourceExecutor();

		~ExecutorsManager() = default;
	};
}
