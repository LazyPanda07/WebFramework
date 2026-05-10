#pragma once

#include "TaskExecutor.h"

#include <Utility/Sources.h>

namespace framework::task_broker
{
	class WEB_FRAMEWORK_API CCTaskExecutor : public TaskExecutor
	{
	private:
		using TaskExecuteSignature = void(*)(void*, void*, void*);
		using DeleteSignature = void(*)(void*);

	private:
		void* implementation;
		TaskExecuteSignature executeFunction;
		DeleteSignature deleteFunction;

	public:
		CCTaskExecutor(HMODULE module, void* implementation, std::string_view taskExecutorName);

		void execute(json::JsonObject& data, TaskExecutorContext& context) override;

		~CCTaskExecutor();
	};
}
