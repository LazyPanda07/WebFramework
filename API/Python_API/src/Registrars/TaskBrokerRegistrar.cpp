#include "Registrars/TaskBrokerRegistrar.h"

#include <pybind11/stl/filesystem.h>

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

		py::class_<framework::task_broker::TaskExecutor::TaskExecutorContext>(m, "TaskExecutorContext")
			.def
			(
				py::init
				(
					[](uint64_t pointer)
					{
						return framework::task_broker::TaskExecutor::TaskExecutorContext(reinterpret_cast<void*>(pointer));
					}
				),
				"pointer"_a
			)
			.def("get_file", &framework::task_broker::TaskExecutor::TaskExecutorContext::getFile, "file_path"_a)
			.def("process_static_file", &framework::task_broker::TaskExecutor::TaskExecutorContext::processStaticFile, "file_data"_a, "file_extension"_a)
			.def
			(
				"process_dynamic_file",
				[](const framework::task_broker::TaskExecutor::TaskExecutorContext& self, std::string_view fileData, py::dict arguments)
				{
					py::module_ json = py::module_::import("json");
					framework::JsonParser parser(json.attr("dumps")(arguments).cast<std::string>());

					self.processDynamicFile(fileData, parser.getParsedData());
				},
				"file_data"_a, "arguments"_a
			)
			.def
			(
				"get_or_create_database",
				[](const framework::task_broker::TaskExecutor::TaskExecutorContext& self, std::string_view databaseName, py::handle databaseImplementationClass)
				{
					if (databaseImplementationClass.is_none() || databaseImplementationClass.is(py::type::of<framework::DefaultDatabase>()))
					{
						return self.getOrCreateDatabase<framework::DefaultDatabase>(databaseName);
					}
					else if (databaseImplementationClass.is(py::type::of<framework::SqliteDatabase>()))
					{
						return self.getOrCreateDatabase<framework::SqliteDatabase>(databaseName);
					}
					else if (databaseImplementationClass.is(py::type::of<framework::RedisDatabase>()))
					{
						return self.getOrCreateDatabase<framework::RedisDatabase>(databaseName);
					}
					else
					{
						throw std::runtime_error(std::format("Wrong database_implementation_class: {}", py::repr(databaseImplementationClass).cast<std::string>()));
					}
				},
				"database_name"_a, "database_implementation_class"_a = py::none()
			)
			.def
			(
				"get_database",
				[](const framework::task_broker::TaskExecutor::TaskExecutorContext& self, std::string_view databaseName, py::handle databaseImplementationClass)
				{
					if (databaseImplementationClass.is_none() || databaseImplementationClass.is(py::type::of<framework::DefaultDatabase>()))
					{
						return self.getDatabase<framework::DefaultDatabase>(databaseName);
					}
					else if (databaseImplementationClass.is(py::type::of<framework::SqliteDatabase>()))
					{
						return self.getDatabase<framework::SqliteDatabase>(databaseName);
					}
					else if (databaseImplementationClass.is(py::type::of<framework::RedisDatabase>()))
					{
						return self.getDatabase<framework::RedisDatabase>(databaseName);
					}
					else
					{
						throw std::runtime_error(std::format("Wrong database_implementation_class: {}", py::repr(databaseImplementationClass).cast<std::string>()));
					}
				},
				"database_name"_a, "database_implementation_class"_a = py::none()
			)
			.def
			(
				"get_or_create_table",
				[](const framework::task_broker::TaskExecutor::TaskExecutorContext& self, std::string_view databaseName, std::string_view tableName, std::string_view createTableQuery, py::handle databaseImplementationClass)
				{
					if (databaseImplementationClass.is_none() || databaseImplementationClass.is(py::type::of<framework::DefaultDatabase>()))
					{
						return self.getOrCreateTable<framework::DefaultDatabase>(databaseName, tableName, createTableQuery);
					}
					else if (databaseImplementationClass.is(py::type::of<framework::SqliteDatabase>()))
					{
						return self.getOrCreateTable<framework::SqliteDatabase>(databaseName, tableName, createTableQuery);
					}
					else if (databaseImplementationClass.is(py::type::of<framework::RedisDatabase>()))
					{
						return self.getOrCreateTable<framework::RedisDatabase>(databaseName, tableName, createTableQuery);
					}
					else
					{
						throw std::runtime_error(std::format("Wrong database_implementation_class: {}", py::repr(databaseImplementationClass).cast<std::string>()));
					}
				},
				"database_name"_a, "table_name"_a, "create_table_query"_a, "database_implementation_class"_a = py::none()
			)
			.def
			(
				"get_table",
				[](const framework::task_broker::TaskExecutor::TaskExecutorContext& self, std::string_view databaseName, std::string_view tableName, py::handle databaseImplementationClass)
				{
					if (databaseImplementationClass.is_none() || databaseImplementationClass.is(py::type::of<framework::DefaultDatabase>()))
					{
						return self.getTable<framework::DefaultDatabase>(databaseName, tableName);
					}
					else if (databaseImplementationClass.is(py::type::of<framework::SqliteDatabase>()))
					{
						return self.getTable<framework::SqliteDatabase>(databaseName, tableName);
					}
					else if (databaseImplementationClass.is(py::type::of<framework::RedisDatabase>()))
					{
						return self.getTable<framework::RedisDatabase>(databaseName, tableName);
					}
					else
					{
						throw std::runtime_error(std::format("Wrong database_implementation_class: {}", py::repr(databaseImplementationClass).cast<std::string>()));
					}
				},
				"database_name"_a, "table_name"_a, "database_implementation_class"_a = py::none()
			);

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
			.def("execute", &framework::task_broker::IPyTaskExecutor::execute, "data"_a, "context"_a);

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
