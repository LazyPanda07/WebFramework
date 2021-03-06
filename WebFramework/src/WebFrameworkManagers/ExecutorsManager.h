#pragma once

#include <unordered_map>
#include <string>
#include <optional>
#include <mutex>
#include <memory>

#include "Executors/BaseExecutor.h"
#include "Utility/JSONSettingsParser.h"
#include "Executors/ResourceExecutor.h"
#include "Utility/RouteParameters.h"

namespace framework
{
	/// <summary>
	/// Managing requests
	/// <para>Initializing subclasses of BaseStatefulExecutor</para>
	/// <para>Holding pointer to ResourceExecutor</para>
	/// <para>Initialize only through WebFramework</para>
	/// </summary>
	class ExecutorsManager
	{
	private:
		std::mutex checkExecutor;
		std::unordered_map<std::string, smartPointer<BaseExecutor>> routes;	// route - executor
		std::unordered_map<std::string, createBaseExecutorSubclassFunction> creator;	// executor name - create function
		std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings> settings;	// route - executor settings
		smartPointer<ResourceExecutor> resources;
		std::vector<utility::RouteParameters> routeParameters;	// base routes for parameterize executors

	public:
		ExecutorsManager() = default;

		ExecutorsManager(const ExecutorsManager&) = delete;

		ExecutorsManager& operator = (const ExecutorsManager&) = delete;

		ExecutorsManager(ExecutorsManager&& other) noexcept;

		ExecutorsManager& operator = (ExecutorsManager&& other) noexcept;

		/// <summary>
		/// Initialize method, called from WebFramework
		/// </summary>
		/// <param name="assets">path to assets</param>
		/// <param name="isCaching">is caching resource files</param>
		/// <param name="pathToTemplates">path to templates folder</param>
		/// <param name="routes">routes for all executors</param>
		/// <param name="creator">functions that create executors</param>
		/// <param name="settings">parsed .json file</param>
		void init(const std::filesystem::path& assets, bool isCaching, const std::string& pathToTemplates, std::unordered_map<std::string, smartPointer<BaseExecutor>>&& routes, std::unordered_map<std::string, createBaseExecutorSubclassFunction>&& creator, std::unordered_map<std::string, utility::JSONSettingsParser::ExecutorSettings>&& settings, std::vector<utility::RouteParameters>&& routeParameters) noexcept;

		/// <summary>
		/// Process requests from server
		/// </summary>
		/// <param name="request">request from client</param>
		/// <param name="response">response to client</param>
		/// <param name="statefulExecutors">all stateful executors for client</param>
		/// <exception cref="framework::exceptions::BaseExecutorException"></exception>
		/// <exception cref="framework::exceptions::FileDoesNotExistException"></exception>
		/// <exception cref="std::out_of_range"></exception>
		void service(HTTPRequest&& request, HTTPResponse& response, std::unordered_map<std::string, smartPointer<BaseExecutor>>& statefulExecutors, std::optional<std::function<void(HTTPRequest&&, HTTPResponse&)>>& threadPoolFunction);

		/// <summary>
		/// Getter for ResourceExecutor
		/// </summary>
		/// <returns>ResourceExecutor</returns>
		smartPointer<ResourceExecutor>& getResourceExecutor();

		~ExecutorsManager() = default;
	};
}
