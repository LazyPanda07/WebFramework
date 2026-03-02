#include "PyTaskSerializer.h"

namespace framework::task_broker
{
	py::dict PyTaskSerializerCxx::serializeArguments() const
	{
		PYBIND11_OVERRIDE_PURE
		(
			py::dict,
			IPyTaskSerializer,
			serializeArguments
		);
	}

	std::string_view PyTaskSerializerCxx::getTaskName() const
	{
		PYBIND11_OVERRIDE_PURE
		(
			std::string_view,
			IPyTaskSerializer,
			getTaskName
		);
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

	std::string_view PyTaskSerializerCc::getTaskName() const
	{
		PYBIND11_OVERRIDE_PURE
		(
			std::string_view,
			IPyTaskSerializer,
			getTaskName
		);
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

	std::string_view PyTaskSerializer::getTaskName() const
	{
		PYBIND11_OVERRIDE_PURE
		(
			std::string_view,
			IPyTaskSerializer,
			getTaskName
		);
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

	std::string_view PyTaskSerializerCSharp::getTaskName() const
	{
		PYBIND11_OVERRIDE_PURE
		(
			std::string_view,
			IPyTaskSerializer,
			getTaskName
		);
	}
}
