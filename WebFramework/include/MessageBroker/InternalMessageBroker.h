#pragma once

#include "MessageBroker.h"

namespace framework::message_broker
{
	class InternalMessageBroker : public MessageBroker
	{
	public:
		static constexpr std::string_view messageBrokerName = "Internal";

	public:
		InternalMessageBroker() = default;

		void enqueueTask(const json::JsonObject& data) override;

		constexpr std::string_view getName() const override;

		~InternalMessageBroker() = default;
	};
}
