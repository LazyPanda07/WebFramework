#pragma once

#include <TaskBroker/TaskSerializer.hpp>

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

		virtual std::string_view getTaskName() const = 0;

		virtual ~IPyTaskSerializer() = default;
	};

	class PyTaskSerializerCxx : public IPyTaskSerializer
	{
	protected:
		py::dict serializeArguments() const override;

		std::string_view getTaskName() const override;

	public:
		PyTaskSerializerCxx() = default;

		~PyTaskSerializerCxx() = default;
	};

	class PyTaskSerializerCc : public IPyTaskSerializer
	{
	protected:
		py::dict serializeArguments() const override;

		std::string_view getTaskName() const override;

	public:
		PyTaskSerializerCc() = default;

		~PyTaskSerializerCc() = default;
	};

	class PyTaskSerializer : public IPyTaskSerializer
	{
	protected:
		py::dict serializeArguments() const override;

		std::string_view getTaskName() const override;

	public:
		PyTaskSerializer() = default;

		~PyTaskSerializer() = default;
	};

	class PyTaskSerializerCSharp : public IPyTaskSerializer
	{
	protected:
		py::dict serializeArguments() const override;

		std::string_view getTaskName() const override;

	public:
		PyTaskSerializerCSharp() = default;

		~PyTaskSerializerCSharp() = default;
	};

	template<typename T>
	class PyTaskSerializerWrapper : public TaskSerializer<T>
	{
	protected:
		JsonObject serializeArguments() const override;

		std::string_view getTaskName() const override;

	private:
		py::object serializer;
		std::string taskName;

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
		taskName(serializer.attr("get_task_name")().cast<std::string>())
	{

	}

	template<typename T>
	JsonObject PyTaskSerializerWrapper<T>::serializeArguments() const
	{
		py::module_ json = py::module_::import("json");

		return JsonParser(json.attr("dumps")(serializer.attr("serialize_arguments")()).cast<std::string>()).getParsedData(false);
	}

	template<typename T>
	std::string_view PyTaskSerializerWrapper<T>::getTaskName() const
	{
		return taskName;
	}
}
