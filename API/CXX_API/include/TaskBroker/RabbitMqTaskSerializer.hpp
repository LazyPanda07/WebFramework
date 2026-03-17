#pragma once

#include "TaskSerializer.hpp"

namespace framework::task_broker
{
	template<TaskBrokerApiImplementation T = CXXApi>
	class RabbitMqTaskSerializer : public TaskSerializer<T>
	{
	protected:
		virtual std::string_view getQueueName() const = 0;

		virtual void serializeMetaData(JsonObject& taskData) const override;

	public:
		RabbitMqTaskSerializer() = default;

		virtual ~RabbitMqTaskSerializer() = default;
	};
}

namespace framework::task_broker
{
	template<TaskBrokerApiImplementation T>
	void RabbitMqTaskSerializer<T>::serializeMetaData(JsonObject& taskData) const
	{
		taskData["queue"] = this->getQueueName();
	}
}
