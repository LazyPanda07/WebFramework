#pragma once

#include "TaskBroker.h"

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
			Client(std::string_view host, int port, amqp_channel_t channel);

			~Client();
		};

	private:
		Client producer;
		Client consumer;

	public:
		RabbitMqTaskBroker();

		void enqueueTask(json::JsonObject&& data) override;

		std::optional<json::JsonObject> requestTask() override;

		std::string_view getName() const override;

		~RabbitMqTaskBroker() = default;
	};
}
