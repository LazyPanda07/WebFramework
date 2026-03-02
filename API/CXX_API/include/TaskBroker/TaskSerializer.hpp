#pragma once

#include <string>
#include <concepts>

#include "TaskApis.hpp"
#include "JsonObject.hpp"

namespace framework::task_broker
{
	template<typename T>
	concept TaskBrokerApiImplementation = requires
	{
		{ std::string(T::taskBrokerApi) } -> std::same_as<std::string>;
	};

	template<TaskBrokerApiImplementation T = CXXApi>
	class TaskSerializer
	{
	protected:
		virtual JsonObject serializeArguments() const = 0;

		virtual constexpr std::string_view getTaskName() const = 0;

	public:
		TaskSerializer() = default;

		JsonObject serialize() const;

		virtual ~TaskSerializer() = default;
	};

	template<typename T>
	concept DerivedFromTaskSerializer = std::derived_from<T, TaskSerializer<CXXApi>>	||
										std::derived_from<T, TaskSerializer<CCApi>>		||
										std::derived_from<T, TaskSerializer<PythonApi>> ||
										std::derived_from<T, TaskSerializer<CSharpApi>>;
}

namespace framework::task_broker
{
	template<TaskBrokerApiImplementation T>
	inline JsonObject TaskSerializer<T>::serialize() const
	{
		JsonObject arguments = this->serializeArguments();

		arguments["api"] = T::taskBrokerApi;
		arguments["name"] = this->getTaskName();

		return arguments;
	}
}
