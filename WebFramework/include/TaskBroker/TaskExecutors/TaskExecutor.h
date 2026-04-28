#pragma once

#include <JsonObject.h>

#include <DatabaseInterfaces/IDatabase.h>

#include "Executors/ResourceExecutor.h"

namespace framework
{
	class WebFramework;
}

namespace framework::task_broker
{
	class TaskExecutor
	{
	public:
		struct TaskExecutorContext
		{
			std::vector<interfaces::IDatabase*> databases;
			std::shared_ptr<ResourceExecutor> resources;
			WebFramework& frameworkInstance;

			TaskExecutorContext(std::shared_ptr<ResourceExecutor> resources, WebFramework& frameworkInstance);

			~TaskExecutorContext();
		};

	public:
		TaskExecutor() = default;

		virtual void execute(json::JsonObject& data, TaskExecutorContext& context) = 0;

		~TaskExecutor() = default;
	};

	using CreateTaskExecutorSignature = void* (*)();
}
