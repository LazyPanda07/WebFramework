#pragma once

#include <unordered_map>
#include <string>

#include <mutex>
#include <memory>

#include <type_traits>

#include <stdexcept>

#include "Executors/BaseExecutor.h"
#include "Utility/XMLSettingsParser.h"

namespace framework
{
	class ExecutorsManager
	{
	private:
		std::mutex checkExecutor;
		std::unordered_map<std::string, std::unique_ptr<BaseExecutor>> routes;	//route - executor
		std::unordered_map<std::string, createBaseExecutorSubclassFunction> creator;	//executor name - create function
		std::unordered_map<std::string, utility::XMLSettingsParser::ExecutorSettings> settings;	//route - executor settings

	private:
		ExecutorsManager(const ExecutorsManager&) = delete;

		ExecutorsManager& operator = (const ExecutorsManager&) = delete;

		ExecutorsManager(ExecutorsManager&&) noexcept = delete;

		ExecutorsManager& operator = (ExecutorsManager&&) noexcept = delete;

	public:
		ExecutorsManager() = default;

		void init(std::unordered_map<std::string, std::unique_ptr<BaseExecutor>>&& routes, std::unordered_map<std::string, createBaseExecutorSubclassFunction>&& creator, std::unordered_map<std::string, utility::XMLSettingsParser::ExecutorSettings>&& settings) noexcept;

		template<typename BaseExecutorSubclass, typename... Args>
		void addRoute(const std::string& route, Args&&... args);

		void service(HTTPRequest&& request, HTTPResponse& response);

		~ExecutorsManager() = default;
	};

	template<typename BaseExecutorSubclass, typename... Args>
	void ExecutorsManager::addRoute(const std::string& route, Args&&... args)
	{
		static_assert(std::is_base_of_v<BaseExecutor, BaseExecutorSubclass>, "BaseExecutorSubclass must be subclass of BaseExecutor");
		
		routes.insert(std::make_pair(route, std::make_unique<BaseExecutorSubclass>(std::forward<Args>(args)...)));
	}
}