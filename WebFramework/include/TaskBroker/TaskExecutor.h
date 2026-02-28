#pragma once

#include <JsonObject.h>

namespace framework::task_broker
{
	class TaskExecutor
	{
	public:
		TaskExecutor() = default;

		virtual void execute(json::JsonObject&& data) = 0;

		~TaskExecutor() = default;
	};
}
