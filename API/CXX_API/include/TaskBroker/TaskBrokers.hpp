#pragma once

#include <string_view>

namespace framework::task_broker
{
	struct Internal
	{
		static inline constexpr std::string_view taskBrokerName = "Internal";
	};

	struct RabbitMq
	{
		static inline constexpr std::string_view taskBrokerName = "RabbitMQ";
	};

	struct ApacheKafka
	{
		static inline constexpr std::string_view taskBrokerName = "Apache Kafka";
	};
}
