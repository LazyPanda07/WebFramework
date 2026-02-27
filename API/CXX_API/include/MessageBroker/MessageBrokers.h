#pragma once

#include <string_view>

namespace framework::message_brokers
{
	struct Internal
	{
		static inline constexpr std::string_view messageBrokerName = "Internal";
	};

	struct RabbitMq
	{
		static inline constexpr std::string_view messageBrokerName = "RabbitMQ";
	};

	struct ApacheKafka
	{
		static inline constexpr std::string_view messageBrokerName = "Apache Kafka";
	};
}
