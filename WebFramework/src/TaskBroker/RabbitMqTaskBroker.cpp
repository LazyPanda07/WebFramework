#include "TaskBroker/RabbitMqTaskBroker.h"

#include <iostream>

#include <JsonParser.h>

#ifdef __LINUX__
#include <sys/time.h>
#else
#include <WinSock2.h>
#endif

namespace framework::task_broker
{
	RabbitMqTaskBroker::Client::Client(std::string_view host, int port, amqp_channel_t channel) :
		connection(amqp_new_connection()),
		socket(amqp_tcp_socket_new(connection)),
		channel(channel)
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

		RabbitMqTaskBroker::callAmqpFunction(amqp_channel_open, *this, connection, channel);
	}

	RabbitMqTaskBroker::Client::~Client()
	{
		amqp_channel_close(connection, channel, AMQP_REPLY_SUCCESS);
		amqp_connection_close(connection, AMQP_REPLY_SUCCESS);
		amqp_destroy_connection(connection);
	}

	template<typename FunctionT, typename... Args>
	void RabbitMqTaskBroker::callAmqpFunction(const FunctionT& function, Client& client, Args&&... args)
	{
		using ReturnType = decltype(function(std::forward<Args>(args)...));

		function(std::forward<Args>(args)...);

		if constexpr (!std::same_as<ReturnType, amqp_rpc_reply_t>)
		{
			amqp_get_rpc_reply(client.connection);
		}
	}

	RabbitMqTaskBroker::RabbitMqTaskBroker() :
		producer("localhost", 5672, 1),
		consumer("localhost", 5672, 2)
	{
		RabbitMqTaskBroker::callAmqpFunction(amqp_queue_declare, producer, producer.connection, producer.channel, amqp_cstring_bytes("test"), 0, 0, 0, 1, amqp_empty_table);

		RabbitMqTaskBroker::callAmqpFunction(amqp_basic_consume, consumer, consumer.connection, consumer.channel, amqp_cstring_bytes("test"), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
	}

	void RabbitMqTaskBroker::enqueueTask(json::JsonObject&& data)
	{
		std::ostringstream stream;
		
		stream << data;

		amqp_basic_publish
		(
			producer.connection,
			producer.channel,
			amqp_cstring_bytes(""),
			amqp_cstring_bytes("test"),
			0,
			0,
			nullptr,
			amqp_cstring_bytes(stream.str().data())
		);
	}

	std::optional<json::JsonObject> RabbitMqTaskBroker::requestTask()
	{
		timeval timeout = { .tv_sec = 0, .tv_usec = 0 };
		amqp_envelope_t envelope;

		amqp_maybe_release_buffers(consumer.connection);

		amqp_rpc_reply_t reply = amqp_consume_message(consumer.connection, &envelope, &timeout, 0);

		if (reply.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION)
		{
			if (reply.library_error == AMQP_STATUS_TIMEOUT)
			{
				return std::nullopt;
			}

			throw std::runtime_error(amqp_error_string2(reply.library_error));
		}

		json::JsonParser parser(std::string_view(static_cast<char*>(envelope.message.body.bytes), envelope.message.body.len));

		amqp_destroy_envelope(&envelope);

		return parser.getParsedData();
	}

	std::string_view RabbitMqTaskBroker::getName() const
	{
		return RabbitMqTaskBroker::taskBrokerName;
	}
}
