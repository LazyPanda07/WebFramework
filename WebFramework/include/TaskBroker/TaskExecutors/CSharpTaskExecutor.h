#pragma once

#include "TaskExecutor.h"

#ifdef __WITH_DOTNET_EXECUTORS__

namespace framework::task_broker
{
	class CSharpTaskExecutor : public TaskExecutor
	{
	private:
		void* implementation;

	public:
		CSharpTaskExecutor(void* implementation);

		void execute(json::JsonObject& data, TaskExecutorContext& context) override;

		~CSharpTaskExecutor();
	};
}

#endif
