#pragma once

#include <unordered_map>
#include <concepts>

#include "Runtimes/Runtime.h"
#include "Utility/DynamicLibraries.h"

namespace framework::runtime
{
	class RuntimesManager
	{
	public:
		class Iterator
		{
		private:
			std::unordered_map<size_t, Runtime*>::iterator it;

		public:
			Iterator(std::unordered_map<size_t, Runtime*>::iterator it);

			Iterator& operator ++();

			bool operator == (const Iterator& other) const noexcept;

			Runtime& operator *();

			const Runtime& operator *() const;

			Runtime* operator ->();

			const Runtime* operator ->() const;

			~Iterator() = default;
		};

	private:
		std::unordered_map<size_t, Runtime*> runtimes; // leaky singleton

	private:
		RuntimesManager() = default;

		RuntimesManager(const RuntimesManager&) = delete;

		RuntimesManager(RuntimesManager&&) noexcept = delete;

		RuntimesManager& operator =(const RuntimesManager&) = delete;

		RuntimesManager& operator =(RuntimesManager&&) noexcept = delete;

		~RuntimesManager() = default;

	public:
		static RuntimesManager& get();

		Iterator begin();

		Iterator end();

		Runtime& getRuntime(utility::LoadSourceType type);

		const Runtime& getRuntime(utility::LoadSourceType type) const;

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
			runtimes.try_emplace(key, new T(std::forward<Args>(args)...));
		}
	}

	template<std::derived_from<Runtime> T>
	T& RuntimesManager::getRuntime()
	{
		return *static_cast<T*>(runtimes.at(typeid(T).hash_code()));
	}

	template<std::derived_from<Runtime> T>
	const T& RuntimesManager::getRuntime() const
	{
		return *static_cast<T*>(runtimes.at(typeid(T).hash_code()));
	}
}
