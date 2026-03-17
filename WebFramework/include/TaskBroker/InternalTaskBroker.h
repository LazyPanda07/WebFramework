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
		InternalTaskBroker(const json::JsonObject& settings);

		void enqueueTask(json::JsonObject&& data) override;

		std::optional<json::JsonObject> requestTask() override;

		std::string_view getName() const override;

		~InternalTaskBroker() = default;
	};
}
