#include "Registrars/SqlRegistrar.h"

#include <fstream>

#include <pybind11/stl.h>

#include <Databases/Database.hpp>
#include <Databases/Implementations/DefaultDatabase.hpp>
#include <Databases/Implementations/RedisDatabase.hpp>
#include <Databases/Implementations/SqliteDatabase.hpp>

#include "PyDTO.h"

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
			.def("execute", [](framework::Table& table, std::string_view query, const std::vector<framework::SqlValue>& values) { return table.execute(query, values); }, "query"_a, "values"_a = py::list())
			.def
			(
				"execute",
				[](framework::Table& table, std::string_view query, py::type dtoClass, const std::vector<framework::SqlValue>& values) -> py::list
				{
					if (dtoClass.is_none())
					{
						throw std::runtime_error("dto_class can't be None");
					}

					if (!py::hasattr(dtoClass, "create"))
					{
						throw std::runtime_error("Can't find create function");
					}

					py::object create = dtoClass.attr("__dict__")["create"];

					if (!py::isinstance<py::staticmethod>(create))
					{
						throw std::runtime_error("Can't find @staticmethod create");
					}

					py::module_ typing = py::module_::import("typing");
					py::object hints = typing.attr("get_type_hints")(create);

					if (!hints.contains("row"))
					{
						throw std::runtime_error("Can't find row parameter");
					}

					py::module_ builtins = py::module_::import("builtins");
					py::module_ webFrameworkApi = py::module_::import("web_framework_api");
					py::object dictType = builtins.attr("dict");
					py::object strType = builtins.attr("str");
					py::object sqlValueType = webFrameworkApi.attr("SqlValue");

					py::object row = hints["row"];

					py::object origin = typing.attr("get_origin")(row);
					py::tuple args = typing.attr("get_args")(row);

					if (!origin.is(dictType))
					{
						throw std::runtime_error("row is not dict");
					}

					if (args.size() != 2)
					{
						throw std::runtime_error("row is not Dict[,]");
					}

					py::object keyType = args[0];
					py::object valueType = args[1];

					if (!keyType.is(strType))
					{
						throw std::runtime_error("row is not Dict[str,]");
					}

					if (!valueType.is(sqlValueType))
					{
						throw std::runtime_error("row is not Dict[str, SqlValue]");
					}

					if (!hints.contains("return"))
					{
						throw std::runtime_error("Can't find return type");
					}

					py::object returnType = hints["return"];

					if (!returnType.is(dtoClass))
					{
						throw std::runtime_error("return type is not " + py::repr(dtoClass).cast<std::string>());
					}

					std::vector<framework::PyDTO> temp = table.execute<framework::PyDTO>(query, values);
					py::list result;

					for (const framework::PyDTO& data : temp)
					{
						result.append(dtoClass.attr("create")(*data));
					}

					return result;
				},
				"query"_a, "dto_class"_a, "values"_a = py::list()
			);

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
