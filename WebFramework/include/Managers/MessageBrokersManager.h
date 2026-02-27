#pragma once

#include <unordered_map>
#include <format>

#include "MessageBroker/MessageBroker.h"

namespace framework::message_broker
{
	class MessageBrokersManager
	{
	private:
		std::unordered_map<size_t, std::unique_ptr<MessageBroker>> messageBrokers;

	private:
		MessageBrokersManager() = default;

		MessageBrokersManager(const MessageBrokersManager&) = delete;

		MessageBrokersManager(MessageBrokersManager&&) noexcept = delete;

		MessageBrokersManager& operator =(const MessageBrokersManager&) = delete;

		MessageBrokersManager& operator =(MessageBrokersManager&&) noexcept = delete;

		~MessageBrokersManager() = default;

	public:
		static MessageBrokersManager& get();

		template<std::derived_from<MessageBroker> T, typename... Args>
		void addMessageBroker(Args&&... args);

		MessageBroker& getMessageBroker(std::string_view name);

		const MessageBroker& getMessageBroker(std::string_view name) const;

		template<std::derived_from<MessageBroker> T>
		T& getMessageBroker();

		template<std::derived_from<MessageBroker> T>
		const T& getMessageBroker() const;
	};
}

namespace framework::message_broker
{
	template<std::derived_from<MessageBroker> T, typename... Args>
	void MessageBrokersManager::addMessageBroker(Args&&... args)
	{
		size_t key = typeid(T).hash_code();

		if (!messageBrokers.contains(key))
		{
			messageBrokers.try_emplace(key, std::make_unique<T>(std::forward<Args>(args)...));
		}
	}

	template<std::derived_from<MessageBroker> T>
	T& MessageBrokersManager::getMessageBroker()
	{
		size_t key = typeid(T).hash_code();

		if (auto it = messageBrokers.find(key); it != messageBrokers.end())
		{
			return *static_cast<T*>(it->second);
		}

		throw std::runtime_error(std::format("Can't find {} message broker", T::messageBrokerName));
	}

	template<std::derived_from<MessageBroker> T>
	const T& MessageBrokersManager::getMessageBroker() const
	{
		size_t key = typeid(T).hash_code();

		if (auto it = messageBrokers.find(key); it != messageBrokers.end())
		{
			return *static_cast<T*>(it->second);
		}

		throw std::runtime_error(std::format("Can't find {} message broker", T::messageBrokerName));
	}
}
