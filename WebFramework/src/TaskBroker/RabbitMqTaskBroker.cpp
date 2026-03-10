#include "TaskBroker/RabbitMqTaskBroker.h"

namespace framework::task_broker
{
	RabbitMqTaskBroker::RabbitMqTaskBroker(std::string_view host, int port) :
		connection(amqp_new_connection()),
		socket(amqp_tcp_socket_new(connection))
	{
		if (!socket)
		{
			throw std::runtime_error(std::format("Can't create RabbitMQ socket"));
		}

		if (amqp_socket_open(socket, host.data(), port))
		{
			throw std::runtime_error(std::format("Can't open RabbitMQ socket"));
		}

		// TODO: update login
		amqp_login(connection, "/", AMQP_DEFAULT_MAX_CHANNELS, AMQP_DEFAULT_FRAME_SIZE, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");

		amqp_channel_open(connection, 1);
		amqp_get_rpc_reply(connection);
	}

	void RabbitMqTaskBroker::enqueueTask(json::JsonObject&& data)
	{
		std::ostringstream stream;
		const std::string& queueName = data["queueName"].get<std::string>();

		stream << data;

		// TOOD: publish tasks
	}

	std::optional<json::JsonObject> RabbitMqTaskBroker::requestTask()
	{
		// TODO: consume tasks

		return std::nullopt;
	}

	std::string_view RabbitMqTaskBroker::getName() const
	{
		return RabbitMqTaskBroker::taskBrokerName;
	}

	RabbitMqTaskBroker::~RabbitMqTaskBroker()
	{
		amqp_channel_close(connection, 1, AMQP_REPLY_SUCCESS);
		amqp_connection_close(connection, AMQP_REPLY_SUCCESS);
		amqp_destroy_connection(connection);
	}
}
