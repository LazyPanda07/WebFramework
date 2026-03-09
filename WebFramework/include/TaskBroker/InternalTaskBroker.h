#pragma once

#include "TaskBroker.h"

#include <Utility/ConcurrentQueue.h>

namespace framework::task_broker
{
	class InternalTaskBroker : public TaskBroker
	{
	public:
		static constexpr std::string_view taskBrokerName = "internal";

	private:
		threading::utility::ConcurrentQueue<json::JsonObject> tasks;

	public:
		InternalTaskBroker() = default;

		void enqueueTask(json::JsonObject&& data) override;

		std::optional<json::JsonObject> requestTask() override;

		constexpr std::string_view getName() const override;

		~InternalTaskBroker() = default;
	};
}

namespace framework::task_broker
{
	inline constexpr std::string_view InternalTaskBroker::getName() const
	{
		return InternalTaskBroker::taskBrokerName;
	}
}
