#pragma once

#include <unordered_map>
#include <string>

#include <mutex>
#include <memory>

#include <type_traits>

#include <stdexcept>

#include "Executors/BaseExecutor.h"
#include "Utility/XMLSettingsParser.h"
#include "Executors/ResourceExecutor.h"

namespace framework
{
	/// <summary>
	/// <para>Managing requests</para>
	/// <para>Initializing subclasses of BaseStatefulExecutor</para>
	/// <para>Holding pointer to ResourceExecutor</para>
	/// <para>Initialize only through WebFramework</para>
	/// </summary>
	class ExecutorsManager
	{
	private:
		std::mutex checkExecutor;
		std::unordered_map<std::string, std::unique_ptr<BaseExecutor>> routes;	//route - executor
		std::unordered_map<std::string, createBaseExecutorSubclassFunction> creator;	//executor name - create function
		std::unordered_map<std::string, utility::XMLSettingsParser::ExecutorSettings> settings;	//route - executor settings
		std::unique_ptr<ResourceExecutor> resources;

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
		/// <param name="routes">routes for all executors</param>
		/// <param name="creator">functions that create executors</param>
		/// <param name="settings">parsed .xml file</param>
		void init(const std::filesystem::path& assets, bool isCaching, std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>&& routes, std::unordered_map<std::string, createBaseExecutorSubclassFunction>&& creator, std::unordered_map<std::string, utility::XMLSettingsParser::ExecutorSettings>&& settings) noexcept;

		/// <summary>
		/// Work in progress method
		/// </summary>
		/// <typeparam name="BaseExecutorSubclass">BaseExecutorSubclass type</typeparam>
		/// <typeparam name="...Args">args type</typeparam>
		/// <param name="route">request path</param>
		/// <param name="...args">arguments for non-default constructor</param>
		template<typename BaseExecutorSubclass, typename... Args>
		void addRoute(const std::string& route, Args&&... args);

		/// <summary>
		/// Process requests from server
		/// </summary>
		/// <param name="request">from server</param>
		/// <param name="response">for client</param>
		/// <param name="statefulExecutors">all stateful executors for client</param>
		void service(HTTPRequest&& request, HTTPResponse& response, std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>& statefulExecutors);

		/// <summary>
		/// Getter for ResourceExecutor
		/// </summary>
		/// <returns>ResourceExecutor</returns>
		std::unique_ptr<ResourceExecutor>& getResourceExecutor();

		~ExecutorsManager() = default;
	};

	template<typename BaseExecutorSubclass, typename... Args>
	void ExecutorsManager::addRoute(const std::string& route, Args&&... args)
	{
		static_assert(std::is_base_of_v<BaseExecutor, BaseExecutorSubclass>, "BaseExecutorSubclass must be subclass of BaseExecutor");
		
		routes.insert(std::make_pair(route, std::make_unique<BaseExecutorSubclass>(std::forward<Args>(args)...)));
	}
}
