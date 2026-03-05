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

		void operator ()(json::JsonObject& data) override;

		~CSharpTaskExecutor();
	};
}

#endif
