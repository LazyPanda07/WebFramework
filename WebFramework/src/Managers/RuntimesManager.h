#pragma once

#include <unordered_map>
#include <memory>
#include <concepts>

#include "Runtimes/Runtime.h"

namespace framework::runtime
{
	class RuntimesManager
	{
	private:
		std::unordered_map<size_t, std::unique_ptr<Runtime>> runtimes;

	private:
		RuntimesManager() = default;

		RuntimesManager(const RuntimesManager&) = delete;

		RuntimesManager(RuntimesManager&&) noexcept = delete;

		RuntimesManager& operator =(const RuntimesManager&) = delete;

		RuntimesManager& operator =(RuntimesManager&&) noexcept = delete;

		~RuntimesManager() = default;

	public:
		static RuntimesManager& get();

		template<std::derived_from<Runtime> T, typename... Args>
		void addRuntime(Args&&... args);

		template<std::derived_from<Runtime> T>
		T& getRuntime();

		template<std::derived_from<Runtime> T>
		const T& getRuntime() const;
	};
}

namespace framework::runtime
{
	template<std::derived_from<Runtime> T, typename... Args>
	void RuntimesManager::addRuntime(Args&&... args)
	{
		size_t key = typeid(T).hash_code();

		if (!runtimes.contains(key))
		{
			runtimes.try_emplace(key, std::make_unique<T>(std::forward<Args>(args)...));
		}
	}

	template<std::derived_from<Runtime> T>
	T& RuntimesManager::getRuntime()
	{
		return *static_cast<T*>(runtimes.at(typeid(T).hash_code()).get());
	}

	template<std::derived_from<Runtime> T>
	const T& RuntimesManager::getRuntime() const
	{
		return *static_cast<T*>(runtimes.at(typeid(T).hash_code()).get());
	}
}
