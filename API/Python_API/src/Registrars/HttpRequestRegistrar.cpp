#include "Registrars/HttpRequestRegistrar.h"

#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/stl/filesystem.h>

#include <HttpRequest.hpp>

#include "PyDynamicFunction.h"
#include "PyChunkGenerator.h"
#include "TaskBroker/PyTaskSerializer.h"
#include "TaskBroker/PyTaskExecutor.h"

namespace py = pybind11;

namespace registrar
{
	void registerHttpRequest(pybind11::module_& m)
	{
		using namespace py::literals;

		py::class_<framework::HttpRequest>(m, "HttpRequest")
			.def
			(
				py::init
				(
					[](uint64_t pointer)
					{
						return framework::HttpRequest(reinterpret_cast<framework::interfaces::IHttpRequest*>(pointer));
					}
				),
				"pointer"_a
			)
			.def("get_raw_parameters", &framework::HttpRequest::getRawParameters)
			.def("get_query_parameters", &framework::HttpRequest::getQueryParameters)
			.def("get_http_version", &framework::HttpRequest::getHTTPVersion)
			.def("get_headers", &framework::HttpRequest::getHeaders)
			.def("get_body", &framework::HttpRequest::getBody)
			.def("set_attribute", &framework::HttpRequest::setAttribute, "name"_a, "value"_a)
			.def("get_attribute", &framework::HttpRequest::getAttribute)
			.def("delete_session", &framework::HttpRequest::deleteSession)
			.def("remove_attribute", &framework::HttpRequest::removeAttribute, "name"_a)
			.def("get_cookies", &framework::HttpRequest::getCookies)
			.def("get_multiparts", &framework::HttpRequest::getMultiparts)
			.def("get_large_data", &framework::HttpRequest::getLargeData)
			.def
			(
				"send_asset_file",
				[](framework::HttpRequest& self, std::string_view filePath, framework::HttpResponse& response, py::dict arguments, bool isBinary, std::string_view fileName)
				{
					if (arguments.size())
					{
						py::module_ json = py::module_::import("json");
						framework::JsonParser parser(json.attr("dumps")(arguments).cast<std::string>());

						self.sendAssetFile(filePath, response, parser.getParsedData(), isBinary, fileName);
					}
					else
					{
						self.sendAssetFile(filePath, response, {}, isBinary, fileName);
					}
				},
				"file_path"_a, "response"_a, "arguments"_a = py::dict(), "is_binary"_a = true, "file_name"_a = "",
				R"pbdoc(
Parameters
----------
file_name : str
    Optional parameter for specifying name of file in Content-Disposition HTTP header, ASCII name required.)pbdoc"
			)
			.def
			(
				"send_static_file", &framework::HttpRequest::sendStaticFile, "file_path"_a, "response"_a, "is_binary"_a = true, "file_name"_a = "",
				R"pbdoc(
Parameters
----------
file_name : str
    Optional parameter for specifying name of file in Content-Disposition HTTP header, ASCII name required.)pbdoc"
			)
			.def
			(
				"send_dynamic_file", 
				[](framework::HttpRequest& self, std::string_view filePath, framework::HttpResponse& response, py::dict arguments, bool isBinary, std::string_view fileName)
				{
					py::module_ json = py::module_::import("json");
					framework::JsonParser parser(json.attr("dumps")(arguments).cast<std::string>());

					self.sendDynamicFile(filePath, response, parser.getParsedData(), isBinary, fileName);
				},
				"file_path"_a, "response"_a, "arguments"_a, "is_binary"_a = false, "file_name"_a = "",
				R"pbdoc(
Parameters
----------
file_name : str
    Optional parameter for specifying name of file in Content-Disposition HTTP header, ASCII name required.)pbdoc"
			)
			.def("stream_file", &framework::HttpRequest::streamFile, "file_path"_a, "response"_a, "file_name"_a, "chunk_size"_a = framework::interfaces::IHttpRequest::defaultChunkSize)
			.def
			(
				"register_dynamic_function",
				[](framework::HttpRequest& self, std::string_view functionName, py::type functionClass)
				{
					py::type dynamicFunctionClass = py::module_::import("web_framework_api").attr("DynamicFunction");

					if (!py::module_::import("builtins").attr("issubclass")(functionClass, dynamicFunctionClass).cast<bool>())
					{
						throw std::runtime_error(std::format("Class {} is not subclass of {}", py::repr(functionClass).cast<std::string>(), py::repr(dynamicFunctionClass).cast<std::string>()));
					}

					self.registerDynamicFunctionClass<framework::PyDynamicFunction>(functionName, &functionClass);
				},
				"function_name"_a, "function_class"_a
			)
			.def("unregister_dynamic_function", &framework::HttpRequest::unregisterDynamicFunction, "function_name"_a)
			.def("is_dynamic_function_registered", &framework::HttpRequest::isDynamicFunctionRegistered, "function_name"_a)
			.def("get_file", &framework::HttpRequest::getFile, "file_path"_a)
			.def("process_static_file", &framework::HttpRequest::processStaticFile, "file_data"_a, "file_extension"_a)
			.def
			(
				"process_dynamic_file",
				[](framework::HttpRequest& request, std::string_view fileData, py::dict arguments)
				{
					py::module_ json = py::module_::import("json");
					framework::JsonParser parser(json.attr("dumps")(arguments).cast<std::string>());

					return request.processDynamicFile(fileData, parser.getParsedData());
				},
				"file_data"_a, "arguments"_a
			)
			.def
			(
				"get_json",
				[](const framework::HttpRequest& self)
				{
					py::module_ json = py::module_::import("json");
					const framework::JsonParser& parser = self.getJson();

					return json.attr("loads")((*parser).data()).cast<py::dict>();
				}
			)
			.def("get_chunks", &framework::HttpRequest::getChunks)
			.def("get_raw_request", &framework::HttpRequest::getRawRequest)
			.def("get_client_ip_v4", &framework::HttpRequest::getClientIpV4)
			.def("get_server_ip_v4", &framework::HttpRequest::getServerIpV4)
			.def("get_client_port", &framework::HttpRequest::getClientPort)
			.def("get_server_port", &framework::HttpRequest::getServerPort)
			.def("get_route_parameter", &framework::HttpRequest::getRouteParameter<std::string>, "route_parameter_name"_a)
			.def("get_route_parameter", &framework::HttpRequest::getRouteParameter<bool>, "route_parameter_name"_a)
			.def("get_route_parameter", &framework::HttpRequest::getRouteParameter<int64_t>, "route_parameter_name"_a)
			.def("get_route_parameter", &framework::HttpRequest::getRouteParameter<double>, "route_parameter_name"_a)
			.def
			(
				"throw_exception",
				[](framework::HttpRequest& self, std::string_view errorMessage, framework::ResponseCodes responseCode, std::string_view logCategory = "") { self.throwException(errorMessage, responseCode, logCategory); },
				"error_message"_a, "response_code"_a, "log_category"_a = ""
			)
			.def
			(
				"send_chunks",
				[](framework::HttpRequest& self, framework::HttpResponse& response, framework::utility::PyChunkGenerator& generator)
				{
					framework::utility::ChunkGeneratorWrapper wrapper(generator);

					self.sendChunks(response, wrapper);
				},
				"response"_a, "generator"_a
			)
			.def
			(
				"send_file_chunks",
				[](framework::HttpRequest& self, framework::HttpResponse& response, std::string_view fileName, framework::utility::PyChunkGenerator& generator)
				{
					framework::utility::ChunkGeneratorWrapper wrapper(generator);

					self.sendFileChunks(response, fileName, wrapper);
				},
				"response"_a, "file_name"_a, "generator"_a
			)
			.def("get_method", &framework::HttpRequest::getMethod)
			.def
			(
				"get_or_create_database",
				[](framework::HttpRequest& self, std::string_view databaseName, py::handle databaseImplementationClass)
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
				[](framework::HttpRequest& self, std::string_view databaseName, py::handle databaseImplementationClass)
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
				[](framework::HttpRequest& self, std::string_view databaseName, std::string_view tableName, std::string_view createTableQuery, py::handle databaseImplementationClass)
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
				[](framework::HttpRequest& self, std::string_view databaseName, std::string_view tableName, py::handle databaseImplementationClass)
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
			)
			.def
			(
				"enqueue_task",
				[](framework::HttpRequest& self, py::type messageBrokerClass, py::type serializerClass, py::kwargs kwargs)
				{
					using namespace framework::task_broker;

					py::object serializer = serializerClass(**kwargs);

					if (messageBrokerClass.is(py::type::of<Internal>()))
					{
						if (py::isinstance<PyTaskSerializerCxx>(serializer))
						{
							self.enqueueTask<Internal, PyTaskSerializerWrapper<CXXApi>>(serializer);
						}
						else if (py::isinstance<PyTaskSerializerCc>(serializer))
						{
							self.enqueueTask<Internal, PyTaskSerializerWrapper<CCApi>>(serializer);
						}
						else if (py::isinstance<PyTaskSerializer>(serializer))
						{
							self.enqueueTask<Internal, PyTaskSerializerWrapper<PythonApi>>(serializer);
						}
						else if (py::isinstance<PyTaskSerializerCSharp>(serializer))
						{
							self.enqueueTask<Internal, PyTaskSerializerWrapper<CSharpApi>>(serializer);
						}
						else
						{
							throw std::runtime_error(std::format("Wrong serializer_class: {}", py::repr(serializerClass).cast<std::string>()));
						}
					}
					else if (messageBrokerClass.is(py::type::of<RabbitMq>()))
					{
						if (py::isinstance<PyTaskSerializerCxx>(serializer))
						{
							self.enqueueTask<RabbitMq, PyTaskSerializerWrapper<CXXApi>>(serializer);
						}
						else if (py::isinstance<PyTaskSerializerCc>(serializer))
						{
							self.enqueueTask<RabbitMq, PyTaskSerializerWrapper<CCApi>>(serializer);
						}
						else if (py::isinstance<PyTaskSerializer>(serializer))
						{
							self.enqueueTask<RabbitMq, PyTaskSerializerWrapper<PythonApi>>(serializer);
						}
						else if (py::isinstance<PyTaskSerializerCSharp>(serializer))
						{
							self.enqueueTask<RabbitMq, PyTaskSerializerWrapper<CSharpApi>>(serializer);
						}
						else
						{
							throw std::runtime_error(std::format("Wrong serializer_class: {}", py::repr(serializerClass).cast<std::string>()));
						}
					}
					else
					{
						throw std::runtime_error(std::format("Wrong message_broker_class: {}", py::repr(messageBrokerClass).cast<std::string>()));
					}
				},
				"message_broker_class"_a, "serializer_class"_a
			);
	}
}
