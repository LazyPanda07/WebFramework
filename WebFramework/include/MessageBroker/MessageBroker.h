#pragma once

#include <JsonObject.h>

namespace framework::message_broker
{
	class MessageBroker
	{
	public:
		MessageBroker() = default;

		virtual void enqueueTask(const json::JsonObject& data) = 0;

		virtual constexpr std::string_view getName() const = 0;

		virtual ~MessageBroker() = default;
	};
}
