#include "TaskBroker/PyTaskSerializer.h"

namespace framework::task_broker
{
	std::string IPyTaskSerializer::getQueueName() const
	{
		return "python_queue";
	}

	py::dict PyTaskSerializerCxx::serializeArguments() const
	{
		PYBIND11_OVERRIDE_PURE
		(
			py::dict,
			IPyTaskSerializer,
			serializeArguments
		);
	}

	std::string PyTaskSerializerCxx::getTaskExecutorName() const
	{
		PYBIND11_OVERRIDE_PURE
		(
			std::string,
			IPyTaskSerializer,
			getTaskExecutorName
		);
	}

	std::string PyTaskSerializerCxx::getQueueName() const
	{
		py::gil_scoped_acquire gil;

		if (py::function override = py::get_override(this, "getQueueName"))
		{
			return override().cast<std::string>();
		}

		return "cxx_queue";
	}

	py::dict PyTaskSerializerCc::serializeArguments() const
	{
		PYBIND11_OVERRIDE_PURE
		(
			py::dict,
			IPyTaskSerializer,
			serializeArguments
		);
	}

	std::string PyTaskSerializerCc::getTaskExecutorName() const
	{
		PYBIND11_OVERRIDE_PURE
		(
			std::string,
			IPyTaskSerializer,
			getTaskExecutorName
		);
	}

	std::string PyTaskSerializerCc::getQueueName() const
	{
		py::gil_scoped_acquire gil;

		if (py::function override = py::get_override(this, "getQueueName"))
		{
			return override().cast<std::string>();
		}

		return "cc_queue";
	}

	py::dict PyTaskSerializer::serializeArguments() const
	{
		PYBIND11_OVERRIDE_PURE
		(
			py::dict,
			IPyTaskSerializer,
			serializeArguments
		);
	}

	std::string PyTaskSerializer::getTaskExecutorName() const
	{
		PYBIND11_OVERRIDE_PURE
		(
			std::string,
			IPyTaskSerializer,
			getTaskExecutorName
		);
	}

	std::string PyTaskSerializer::getQueueName() const
	{
		py::gil_scoped_acquire gil;

		if (py::function override = py::get_override(this, "getQueueName"))
		{
			return override().cast<std::string>();
		}

		return "python_queue";
	}

	py::dict PyTaskSerializerCSharp::serializeArguments() const
	{
		PYBIND11_OVERRIDE_PURE
		(
			py::dict,
			IPyTaskSerializer,
			serializeArguments
		);
	}

	std::string PyTaskSerializerCSharp::getTaskExecutorName() const
	{
		PYBIND11_OVERRIDE_PURE
		(
			std::string,
			IPyTaskSerializer,
			getTaskExecutorName
		);
	}

	std::string PyTaskSerializerCSharp::getQueueName() const
	{
		py::gil_scoped_acquire gil;

		if (py::function override = py::get_override(this, "getQueueName"))
		{
			return override().cast<std::string>();
		}

		return "csharp_queue";
	}
}
