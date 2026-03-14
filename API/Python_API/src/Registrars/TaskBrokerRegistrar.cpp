#include "Registrars/TaskBrokerRegistrar.h"

#include <TaskBroker/TaskBrokers.hpp>
#include <TaskBroker/TaskApis.hpp>

#include "TaskBroker/PyTaskExecutor.h"
#include "TaskBroker/PyTaskSerializer.h"

namespace py = pybind11;

namespace registrar
{
	void registerTaskBrokers(pybind11::module_& m)
	{
		using namespace py::literals;

		py::class_<framework::task_broker::CXXApi>(m, "CXXApi")
			.def_property_readonly_static("task_broker_api", [](py::object self) { return std::string(framework::task_broker::CXXApi::taskBrokerApi); });

		py::class_<framework::task_broker::CCApi>(m, "CCApi")
			.def_property_readonly_static("task_broker_api", [](py::object self) { return std::string(framework::task_broker::CCApi::taskBrokerApi); });

		py::class_<framework::task_broker::PythonApi>(m, "PythonApi")
			.def_property_readonly_static("task_broker_api", [](py::object self) { return std::string(framework::task_broker::PythonApi::taskBrokerApi); });

		py::class_<framework::task_broker::CSharpApi>(m, "CSharpApi")
			.def_property_readonly_static("task_broker_api", [](py::object self) { return std::string(framework::task_broker::CSharpApi::taskBrokerApi); });

		py::class_<framework::task_broker::Internal>(m, "Internal")
			.def_property_readonly_static("task_broker_name", [](py::object self) { return std::string(framework::task_broker::Internal::taskBrokerName); });

		py::class_<framework::task_broker::RabbitMq>(m, "RabbitMQ")
			.def_property_readonly_static("task_broker_name", [](py::object self) { return std::string(framework::task_broker::RabbitMq::taskBrokerName); });

		py::class_<framework::task_broker::IPyTaskExecutor, framework::task_broker::PyTaskExecutor>(m, "TaskExecutor")
			.def(py::init())
			.def("__call__", &framework::task_broker::IPyTaskExecutor::operator (), "data"_a);

		py::class_<framework::task_broker::IPyTaskSerializer>(m, "IPyTaskSerializer")
			.def("serialize_arguments", &framework::task_broker::IPyTaskSerializer::serializeArguments)
			.def("get_task_executor_name", &framework::task_broker::IPyTaskSerializer::getTaskExecutorName)
			.def("get_queue_name", &framework::task_broker::IPyTaskSerializer::getQueueName);

		py::class_<framework::task_broker::PyTaskSerializerCxx, framework::task_broker::IPyTaskSerializer>(m, "TaskSerializerCxx")
			.def
			(
				py::init
				(
					[](py::args args, py::kwargs kwargs)
					{
						return new framework::task_broker::PyTaskSerializerCxx();
					}
				)
			);

		py::class_<framework::task_broker::PyTaskSerializerCc, framework::task_broker::IPyTaskSerializer>(m, "TaskSerializerCc")
			.def
			(
				py::init
				(
					[](py::args args, py::kwargs kwargs)
					{
						return new framework::task_broker::PyTaskSerializerCc();
					}
				)
			);

		py::class_<framework::task_broker::PyTaskSerializer, framework::task_broker::IPyTaskSerializer>(m, "TaskSerializer")
			.def
			(
				py::init
				(
					[](py::args args, py::kwargs kwargs)
					{
						return new framework::task_broker::PyTaskSerializer();
					}
				)
			);

		py::class_<framework::task_broker::PyTaskSerializerCSharp, framework::task_broker::IPyTaskSerializer>(m, "TaskSerializerCSharp")
			.def
			(
				py::init
				(
					[](py::args args, py::kwargs kwargs)
					{
						return new framework::task_broker::PyTaskSerializerCSharp();
					}
				)
			);
	}
}
