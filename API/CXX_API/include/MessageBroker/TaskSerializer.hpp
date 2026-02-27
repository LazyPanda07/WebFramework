#pragma once

#include <string>
#include <concepts>

#include "TaskApis.hpp"
#include "JsonObject.hpp"

namespace framework::message_broker
{
	template<typename T>
	concept MessageBrokerApiImplementation = requires
	{
		{ std::string(T::messageBrokerApi) } -> std::same_as<std::string>;
	};

	template<MessageBrokerApiImplementation T = CXXApi>
	class TaskSerializer
	{
	protected:
		virtual JsonObject serializeArguments() const = 0;

		virtual constexpr std::string_view getName() const = 0;

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

namespace framework::message_broker
{
	template<MessageBrokerApiImplementation T>
	inline JsonObject TaskSerializer<T>::serialize() const
	{
		JsonObject arguments = this->serializeArguments();

		arguments["api"] = T::messageBrokerApi;
		arguments["name"] = this->getName();

		return arguments;
	}
}
