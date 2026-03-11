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
	RabbitMqTaskBroker::Client::Client(std::string_view host, int port) :
		connection(amqp_new_connection()),
		socket(amqp_tcp_socket_new(connection))
	{
		if (!socket)
		{
			throw std::runtime_error(std::format("Can't create RabbitMQ socket"));
		}

		if (amqp_socket_open(socket, "localhost", 5672))
		{
			throw std::runtime_error(std::format("Can't open RabbitMQ socket"));
		}

		// TODO: update login
		amqp_login(connection, "/", AMQP_DEFAULT_MAX_CHANNELS, AMQP_DEFAULT_FRAME_SIZE, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");

		amqp_channel_open(connection, 1);
		amqp_get_rpc_reply(connection);

		{
			amqp_queue_declare_ok_t* r = amqp_queue_declare(connection, 1, amqp_empty_bytes, 0, 0, 0, 1, amqp_empty_table);

			amqp_get_rpc_reply(connection);

			queueName = amqp_bytes_malloc_dup(r->queue);

			if (queueName.bytes == NULL)
			{
				fprintf(stderr, "Out of memory while copying queue name");

				exit(5);
			}
		}
	}

	RabbitMqTaskBroker::Client::~Client()
	{
		amqp_channel_close(connection, 1, AMQP_REPLY_SUCCESS);
		amqp_connection_close(connection, AMQP_REPLY_SUCCESS);
		amqp_destroy_connection(connection);

		amqp_bytes_free(queueName);
	}

	RabbitMqTaskBroker::RabbitMqTaskBroker() :
		producer("localhost", 5672),
		consumer("localhost", 5672)
	{
		amqp_queue_declare(producer.connection, 1, amqp_cstring_bytes("test"), 0, 0, 0, 1, amqp_empty_table);

		amqp_basic_consume(consumer.connection, 1, amqp_cstring_bytes("test"), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
	}

	void RabbitMqTaskBroker::enqueueTask(json::JsonObject&& data)
	{
		std::ostringstream stream;
		
		stream << data;

		amqp_basic_publish
		(
			producer.connection,
			1,
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
		amqp_rpc_reply_t reply;
		amqp_envelope_t envelope;

		amqp_maybe_release_buffers(consumer.connection);

		reply = amqp_consume_message(consumer.connection, &envelope, &timeout, 0);

		if (reply.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION)
		{
			std::cerr << amqp_error_string2(reply.library_error) << std::endl;

			return std::nullopt;
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
