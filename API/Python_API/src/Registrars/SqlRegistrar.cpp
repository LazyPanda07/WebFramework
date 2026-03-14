#include "Registrars/SqlRegistrar.h"

#include <pybind11/stl.h>

#include <Databases/Database.hpp>
#include <Databases/Implementations/DefaultDatabase.hpp>
#include <Databases/Implementations/RedisDatabase.hpp>
#include <Databases/Implementations/SqliteDatabase.hpp>

namespace py = pybind11;

namespace registrar
{
	void registerSql(pybind11::module_& m)
	{
		using namespace py::literals;

		py::class_<framework::SqlValue>(m, "SqlValue")
			.def(py::init<int64_t>(), "value"_a)
			.def(py::init<double>(), "value"_a)
			.def(py::init<const std::string&>(), "value"_a)
			.def(py::init<std::nullptr_t>(), "value"_a)
			.def(py::init<const std::vector<uint8_t>&>(), "value"_a)
			.def("get", &framework::SqlValue::operator*)
			.def("get_as_int", [](framework::SqlValue& self) { return self.get<int64_t>(); })
			.def("get_as_bool", [](framework::SqlValue& self) { return self.get<bool>(); })
			.def("get_as_float", [](framework::SqlValue& self) { return self.get<double>(); })
			.def("get_as_string", [](framework::SqlValue& self) { return self.get<std::string>(); })
			.def("get_as_blob", [](framework::SqlValue& self) { return self.get<std::vector<uint8_t>>(); });

		py::class_<framework::SqlResult>(m, "SqlResult")
			.def("at", [](const framework::SqlResult& self, size_t index) { return self.at(index); }, "index"_a)
			.def("__len__", &framework::SqlResult::size)
			.def("__iter__", [](framework::SqlResult& self) { return py::make_iterator(self.begin(), self.end()); }, py::keep_alive<0, 1>())
			.def("__getitem__", [](const framework::SqlResult& self, size_t index) { return self[index]; }, "index"_a);

		py::class_<framework::Table>(m, "Table")
			.def("execute", [](framework::Table& table, std::string_view query, const std::vector<framework::SqlValue>& values = {}) { return table.execute(query, values); }, "query"_a, "values"_a = py::list());

		py::class_<framework::Database>(m, "Database")
			.def("__contains__", [](const framework::Database& self, std::string_view tableName) { return self.contains(tableName); }, "table_name"_a)
			.def("get_table", &framework::Database::getTable, "table_name"_a)
			.def("get_or_create_table", &framework::Database::getOrCreateTable, "table_name"_a, "create_table_query"_a)
			.def("get_database_name", &framework::Database::getDatabaseName)
			.def("get_database_file_name", &framework::Database::getDatabaseFileName)
			.def("__getitem__", &framework::Database::getTable, "table_name"_a)
			.def("__contains__", [](const framework::Database& self, std::string_view tableName) { return self.contains(tableName); }, "table_name"_a);

		py::class_<framework::DefaultDatabase>(m, "DefaultDatabase")
			.def_property_readonly_static("database_implementation_name", [](py::object self) { return std::string(framework::DefaultDatabase::databaseImplementationName); });

		py::class_<framework::SqliteDatabase>(m, "SqliteDatabase")
			.def_property_readonly_static("database_implementation_name", [](py::object self) { return std::string(framework::SqliteDatabase::databaseImplementationName); });

		py::class_<framework::RedisDatabase>(m, "RedisDatabase")
			.def_property_readonly_static("database_implementation_name", [](py::object self) { return std::string(framework::RedisDatabase::databaseImplementationName); });
	}
}
