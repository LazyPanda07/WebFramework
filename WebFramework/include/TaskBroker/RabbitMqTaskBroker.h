#pragma once

#include "TaskBroker.h"

#include <unordered_set>

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

namespace framework::task_broker
{
	class RabbitMqTaskBroker : public TaskBroker
	{
	public:
		static constexpr std::string_view taskBrokerName = "rabbitmq";

	private:
		std::unordered_set<std::string> queues;
		amqp_connection_state_t connection;
		amqp_socket_t* socket;

	public:
		RabbitMqTaskBroker(std::string_view host, int port);

		void enqueueTask(json::JsonObject&& data) override;

		std::optional<json::JsonObject> requestTask() override;

		std::string_view getName() const override;

		~RabbitMqTaskBroker();
	};
}
