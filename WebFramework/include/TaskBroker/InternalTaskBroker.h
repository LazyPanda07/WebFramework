#pragma once

#include "TaskBroker.h"

namespace framework::task_broker
{
	class InternalTaskBroker : public TaskBroker
	{
	public:
		static constexpr std::string_view taskBrokerName = "Internal";

	public:
		InternalTaskBroker() = default;

		void enqueueTask(const json::JsonObject& data) override;

		constexpr std::string_view getName() const override;

		~InternalTaskBroker() = default;
	};
}
