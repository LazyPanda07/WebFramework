#pragma once

#include "TaskExecutor.h"

#include <Utility/Sources.h>

namespace framework::task_broker
{
	class CCTaskExecutor : public TaskExecutor
	{
	private:
		using TaskExecuteSignature = void(*)(void*, void*);
		using DeleteSignature = void(*)(void*);

	private:
		void* implementation;
		TaskExecuteSignature executeFunction;
		DeleteSignature deleteFunction;

	public:
		CCTaskExecutor(HMODULE module, void* implementation, std::string_view taskExecutorName);

		void operator ()(json::JsonObject& data) override;

		~CCTaskExecutor();
	};
}
