#pragma once

#include "TaskBroker.h"

#include <unordered_set>
#include <shared_mutex>

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

namespace framework::task_broker
{
	class RabbitMqTaskBroker : public TaskBroker
	{
	public:
		static constexpr std::string_view taskBrokerName = "rabbitmq";

	private:
		struct Client
		{
			amqp_connection_state_t connection;
			amqp_socket_t* socket;
			amqp_channel_t channel;

		public:
			Client(const json::JsonObject& settings, amqp_channel_t channel);

			~Client();
		};

	private:
		template<typename FunctionT, typename... Args>
		static void callAmqpFunction(const FunctionT& function, Client& client, Args&&... args);

	private:
		std::unordered_set<std::string> queues;
		std::shared_mutex readWriteMutex;
		Client producer;
		Client consumer;

	public:
		RabbitMqTaskBroker(const json::JsonObject& settings);

		void enqueueTask(json::JsonObject&& data) override;

		std::optional<json::JsonObject> requestTask() override;

		std::string_view getName() const override;

		~RabbitMqTaskBroker() = default;
	};
}
