#ifndef __LINUX__
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "TaskBroker/RabbitMqTaskBroker.h"

#include <JsonParser.h>

#ifdef __LINUX__
#include <sys/time.h>
#else
#include <WinSock2.h>
#endif

#include "Framework/WebFrameworkConstants.h"
#include "Utility/Utils.h"

namespace framework::task_broker
{
	RabbitMqTaskBroker::Client::Client(const json::JsonObject& settings, amqp_channel_t channel) :
		connection(amqp_new_connection()),
		socket(amqp_tcp_socket_new(connection)),
		channel(channel)
	{
		if (!socket)
		{
			utility::logAndThrowException<logging::message::cantCreateRabbitMqSocket, logging::category::taskBroker>();
		}

		std::string host(json_settings_values::rabbitMqHostValue);
		int port = json_settings_values::rabbitMqPortValue;

		settings.tryGet<std::string>(json_settings::rabbitMqHostKey, host);
		settings.tryGet<int>(json_settings::rabbitMqPortKey, port);

		if (Log::isValid())
		{
			Log::info<logging::message::openAmqpSocket, logging::category::taskBroker>(host, port);
		}

		if (amqp_socket_open(socket, host.data(), port))
		{
			utility::logAndThrowException<logging::message::cantOpenRabbitMqSocket, logging::category::taskBroker>();
		}

		std::string login("guest");
		std::string password("guest");

		if (const char* loginEnv = std::getenv("RABBIT_MQ_LOGIN"))
		{
			login = loginEnv;
		}

		if (const char* passwordEnv = std::getenv("RABBIT_MQ_PASSWORD"))
		{
			password = passwordEnv;
		}

		if (Log::isValid())
		{
			Log::info<logging::message::rabbitmqLogin, logging::category::taskBroker>(channel);
		}

		amqp_rpc_reply_t reply = amqp_login(connection, "/", AMQP_DEFAULT_MAX_CHANNELS, AMQP_DEFAULT_FRAME_SIZE, 0, AMQP_SASL_METHOD_PLAIN, login.data(), password.data());

		if (reply.library_error)
		{
			utility::logAndThrowException<logging::message::rabbitmqLoginError, logging::category::taskBroker>(amqp_error_string2(reply.library_error));
		}

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
			amqp_rpc_reply_t reply = amqp_get_rpc_reply(client.connection);

			if (reply.library_error)
			{
				if (Log::isValid())
				{
					Log::error<logging::message::amqpFunctionCallException, logging::category::taskBroker>(amqp_error_string2(reply.library_error));
				}
			}
		}
	}

	RabbitMqTaskBroker::RabbitMqTaskBroker(const json::JsonObject& settings) :
		producer(settings, 1),
		consumer(settings, 2)
	{
		
	}

	void RabbitMqTaskBroker::enqueueTask(json::JsonObject&& data)
	{
		bool hasQueue = false;
		const std::string& queue = data.at("queue").get<std::string>();

		{
			std::shared_lock<std::shared_mutex> lock(readWriteMutex);

			hasQueue = queues.contains(queue);
		}

		if (!hasQueue)
		{
			std::unique_lock<std::shared_mutex> lock(readWriteMutex);

			RabbitMqTaskBroker::callAmqpFunction(amqp_queue_declare, producer, producer.connection, producer.channel, amqp_cstring_bytes(queue.data()), 0, 0, 0, 1, amqp_empty_table);
			RabbitMqTaskBroker::callAmqpFunction(amqp_basic_consume, consumer, consumer.connection, consumer.channel, amqp_cstring_bytes(queue.data()), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);

			queues.insert(queue);
		}

		std::ostringstream stream;

		stream << data;

		amqp_basic_publish
		(
			producer.connection,
			producer.channel,
			amqp_cstring_bytes(""),
			amqp_cstring_bytes(queue.data()),
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

			utility::logAndThrowException<logging::message::rabbitMqRequestTaskException, logging::category::taskBroker>(amqp_error_string2(reply.library_error));
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
