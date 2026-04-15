#pragma once

#include <TaskBroker/RabbitMqTaskSerializer.hpp>

#include <JsonParser.hpp>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace framework::task_broker
{
	class IPyTaskSerializer
	{
	public:
		IPyTaskSerializer() = default;

		virtual py::dict serializeArguments() const = 0;

		virtual std::string getTaskExecutorName() const = 0;

		virtual std::string getQueueName() const;

		virtual ~IPyTaskSerializer() = default;
	};

	class PyTaskSerializerCxx : public IPyTaskSerializer
	{
	protected:
		py::dict serializeArguments() const override;

		std::string getTaskExecutorName() const override;

		std::string getQueueName() const override;

	public:
		PyTaskSerializerCxx() = default;

		~PyTaskSerializerCxx() = default;
	};

	class PyTaskSerializerCc : public IPyTaskSerializer
	{
	protected:
		py::dict serializeArguments() const override;

		std::string getTaskExecutorName() const override;

		std::string getQueueName() const override;

	public:
		PyTaskSerializerCc() = default;

		~PyTaskSerializerCc() = default;
	};

	class PyTaskSerializer : public IPyTaskSerializer
	{
	protected:
		py::dict serializeArguments() const override;

		std::string getTaskExecutorName() const override;

		std::string getQueueName() const override;

	public:
		PyTaskSerializer() = default;

		~PyTaskSerializer() = default;
	};

	class PyTaskSerializerCSharp : public IPyTaskSerializer
	{
	protected:
		py::dict serializeArguments() const override;

		std::string getTaskExecutorName() const override;

		std::string getQueueName() const override;

	public:
		PyTaskSerializerCSharp() = default;

		~PyTaskSerializerCSharp() = default;
	};

	template<typename T>
	class PyTaskSerializerWrapper : public RabbitMqTaskSerializer<T>
	{
	protected:
		JsonObject serializeArguments() const override;

		std::string_view getTaskExecutorName() const override;

		std::string_view getQueueName() const override;

	private:
		py::object serializer;
		std::string taskName;
		std::string queueName;

	public:
		PyTaskSerializerWrapper(py::object serializer);

		~PyTaskSerializerWrapper() = default;
	};
}

namespace framework::task_broker
{
	template<typename T>
	PyTaskSerializerWrapper<T>::PyTaskSerializerWrapper(py::object serializer) :
		serializer(serializer),
		taskName(serializer.attr("get_task_executor_name")().cast<std::string>()),
		queueName(serializer.attr("get_queue_name")().cast<std::string>())
	{

	}

	template<typename T>
	JsonObject PyTaskSerializerWrapper<T>::serializeArguments() const
	{
		py::module json = py::module::import("json");

		return JsonParser(json.attr("dumps")(serializer.attr("serialize_arguments")()).cast<std::string>()).getParsedData(false);
	}

	template<typename T>
	std::string_view PyTaskSerializerWrapper<T>::getTaskExecutorName() const
	{
		return taskName;
	}

	template<typename T>
	std::string_view PyTaskSerializerWrapper<T>::getQueueName() const
	{
		return queueName;
	}
}
