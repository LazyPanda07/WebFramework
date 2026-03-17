#pragma once

#include <string_view>

namespace framework::task_broker
{
	struct Internal
	{
		static inline constexpr std::string_view taskBrokerName = "internal";
	};

	struct RabbitMq
	{
		static inline constexpr std::string_view taskBrokerName = "rabbitmq";
	};
}
