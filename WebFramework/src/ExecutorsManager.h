#pragma once

#include <unordered_map>
#include <string>

#include <memory>

#include <type_traits>

#include <stdexcept>

#include "BaseExecutor.h"

namespace framework
{
	class ExecutorsManager
	{
	private:
		std::unordered_map<std::string, std::unique_ptr<BaseExecutor>> routes;

	private:
		ExecutorsManager(const ExecutorsManager&) = delete;

		ExecutorsManager& operator = (const ExecutorsManager&) = delete;

		ExecutorsManager& operator = (ExecutorsManager&&) noexcept = delete;

	public:
		ExecutorsManager() = default;

		ExecutorsManager(ExecutorsManager&& other) noexcept = default;

		template<typename BaseExecutorSubclass, typename... Args>
		void addRoute(const std::string& route, Args&&... args);

		void service(web::HTTPParser&& request, std::string& response);

		~ExecutorsManager() = default;
	};

	template<typename BaseExecutorSubclass, typename... Args>
	void ExecutorsManager::addRoute(const std::string& route, Args&&... args)
	{
		if constexpr (!std::is_base_of_v<BaseExecutor, BaseExecutorSubclass>)
		{
			throw exceptions::ExecutorException("BaseExecutorSubclass must be subclass of BaseExecutor");
		}
		
		routes.insert(std::make_pair(route, std::make_unique<BaseExecutorSubclass>(std::forward<Args>(args)...)));
	}
}