#pragma once

#include <JsonObject.h>

#include "Framework/WebFrameworkPlatform.h"

namespace framework::task_broker
{
	class WEB_FRAMEWORK_API TaskBroker
	{
	public:
		TaskBroker() = default;

		virtual void enqueueTask(json::JsonObject&& data) = 0;

		virtual std::optional<json::JsonObject> requestTask() = 0;

		virtual std::string_view getName() const = 0;

		virtual ~TaskBroker() = default;
	};
}
