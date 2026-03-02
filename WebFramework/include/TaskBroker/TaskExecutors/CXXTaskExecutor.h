#pragma once

#include "TaskExecutor.h"

#include "Utility/Sources.h"

namespace framework::task_broker
{
	class CXXTaskExecutor : public TaskExecutor
	{
	private:
		using TaskExecuteSignature = void(*)(void*, void*);
		using DeleteSignature = void(*)(void*);

	private:
		void* implementation;
		TaskExecuteSignature executeFunction;
		DeleteSignature deleteFunction;

	public:
		CXXTaskExecutor(HMODULE module, void* implementation);

		void operator ()(json::JsonObject& data) override;

		~CXXTaskExecutor();
	};
}
