#pragma once

#include <JsonObject.h>

namespace framework::task_broker
{
	class TaskBroker
	{
	public:
		TaskBroker() = default;

		virtual void enqueueTask(const json::JsonObject& data) = 0;

		virtual constexpr std::string_view getName() const = 0;

		virtual ~TaskBroker() = default;
	};
}
