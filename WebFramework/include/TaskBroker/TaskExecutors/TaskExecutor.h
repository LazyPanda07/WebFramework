#pragma once

#include <JsonObject.h>

namespace framework::task_broker
{
	class TaskExecutor
	{
	public:
		TaskExecutor() = default;

		virtual void operator ()(json::JsonObject& data) = 0;

		~TaskExecutor() = default;
	};

	using CreateTaskExecutorSignature = void* (*)();
}
