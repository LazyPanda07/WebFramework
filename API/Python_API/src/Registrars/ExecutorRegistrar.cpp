#include "Registrars/ExecutorRegistrar.h"

#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

#include <Executors/Executor.hpp>

#include "PyDynamicFunction.h"
#include "Executors/PyStatelessExecutor.h"
#include "Executors/PyHeavyOperationStatelessExecutor.h"
#include "Executors/PyStatefulExecutor.h"
#include "Executors/PyHeavyOperationStatefulExecutor.h"

namespace py = pybind11;

namespace registrar
{
	void registerExecutorSettings(pybind11::module_& m)
	{
		using namespace py::literals;

		py::class_<framework::utility::ExecutorSettings> executorSettings(m, "ExecutorSettings");

		py::enum_<framework::utility::ExecutorSettings::LoadType>(executorSettings, "LoadType")
			.value("initialization", framework::utility::ExecutorSettings::LoadType::initialization)
			.value("dynamic", framework::utility::ExecutorSettings::LoadType::dynamic)
			.value("none", framework::utility::ExecutorSettings::LoadType::none)
			.export_values();

		py::enum_<framework::utility::ExecutorType>(m, "ExecutorType")
			.value("stateful", framework::utility::ExecutorType::stateful)
			.value("stateless", framework::utility::ExecutorType::stateless)
			.value("heavyOperationStateful", framework::utility::ExecutorType::heavyOperationStateful)
			.value("heavyOperationStateless", framework::utility::ExecutorType::heavyOperationStateless)
			.export_values();

		py::enum_<framework::ResponseCodes>(m, "ResponseCodes")
			.value("CONTINUE", framework::ResponseCodes::continueCode)
			.value("SWITCHING_PROTOCOLS", framework::ResponseCodes::switchingProtocols)
			.value("PROCESSING", framework::ResponseCodes::processing)
			.value("OK", framework::ResponseCodes::ok)
			.value("CREATED", framework::ResponseCodes::created)
			.value("ACCEPTED", framework::ResponseCodes::accepted)
			.value("NON_AUTHORITATIVE_INFORMATION", framework::ResponseCodes::nonAuthoritativeInformation)
			.value("NO_CONTENT", framework::ResponseCodes::noContent)
			.value("RESET_CONTENT", framework::ResponseCodes::resetContent)
			.value("PARTIAL_CONTENT", framework::ResponseCodes::partialContent)
			.value("MULTI_STATUS", framework::ResponseCodes::multiStatus)
			.value("ALREADY_REPORTED", framework::ResponseCodes::alreadyReported)
			.value("IM_USED", framework::ResponseCodes::IMUsed)
			.value("MULTIPLE_CHOICES", framework::ResponseCodes::multipleChoices)
			.value("MOVED_PERMANENTLY", framework::ResponseCodes::movedPermanently)
			.value("FOUND", framework::ResponseCodes::found)
			.value("SEE_OTHER", framework::ResponseCodes::seeOther)
			.value("NOT_MODIFIED", framework::ResponseCodes::notModified)
			.value("USE_PROXY", framework::ResponseCodes::useProxy)
			.value("TEMPORARY_REDIRECT", framework::ResponseCodes::temporaryRedirect)
			.value("PERMANENT_REDIRECT", framework::ResponseCodes::permanentRedirect)
			.value("BAD_REQUEST", framework::ResponseCodes::badRequest)
			.value("UNAUTHORIZED", framework::ResponseCodes::unauthorized)
			.value("PAYMENT_REQUIRED", framework::ResponseCodes::paymentRequired)
			.value("FORBIDDEN", framework::ResponseCodes::forbidden)
			.value("NOT_FOUND", framework::ResponseCodes::notFound)
			.value("METHOD_NOT_ALLOWED", framework::ResponseCodes::methodNotAllowed)
			.value("NOT_ACCEPTABLE", framework::ResponseCodes::notAcceptable)
			.value("PROXY_AUTHENTICATION_REQUIRED", framework::ResponseCodes::proxyAuthenticationRequired)
			.value("REQUEST_TIMEOUT", framework::ResponseCodes::requestTimeout)
			.value("CONFLICT", framework::ResponseCodes::conflict)
			.value("GONE", framework::ResponseCodes::gone)
			.value("LENGTH_REQUIRED", framework::ResponseCodes::lengthRequired)
			.value("PRECONDITION_FAILED", framework::ResponseCodes::preconditionFailed)
			.value("PAYLOAD_TOO_LARGE", framework::ResponseCodes::payloadTooLarge)
			.value("URI_TOO_LONG", framework::ResponseCodes::URITooLong)
			.value("UNSUPPORTED_MEDIA_TYPE", framework::ResponseCodes::unsupportedMediaType)
			.value("RANGE_NOT_SATISFIABLE", framework::ResponseCodes::rangeNotSatisfiable)
			.value("EXPECTATION_FAILED", framework::ResponseCodes::expectationFailed)
			.value("IAM_A_TEAPOT", framework::ResponseCodes::iamATeapot)
			.value("AUTHENTICATION_TIMEOUT", framework::ResponseCodes::authenticationTimeout)
			.value("MISDIRECTED_REQUEST", framework::ResponseCodes::misdirectedRequest)
			.value("UNPROCESSABLE_ENTITY", framework::ResponseCodes::unprocessableEntity)
			.value("LOCKED", framework::ResponseCodes::locked)
			.value("FAILED_DEPENDENCY", framework::ResponseCodes::failedDependency)
			.value("UPGRADE_REQUIRED", framework::ResponseCodes::upgradeRequired)
			.value("PRECONDITION_REQUIRED", framework::ResponseCodes::preconditionRequired)
			.value("TOO_MANY_REQUESTS", framework::ResponseCodes::tooManyRequests)
			.value("REQUEST_HEADER_FIELDS_TOO_LARGE", framework::ResponseCodes::requestHeaderFieldsTooLarge)
			.value("RETRY_WITH", framework::ResponseCodes::retryWith)
			.value("UNAVAILABLE_FOR_LEGAL_REASONS", framework::ResponseCodes::unavailableForLegalReasons)
			.value("CLIENT_CLOSED_REQUEST", framework::ResponseCodes::clientClosedRequest)
			.value("INTERNAL_SERVER_ERROR", framework::ResponseCodes::internalServerError)
			.value("NOT_IMPLEMENTED", framework::ResponseCodes::notImplemented)
			.value("BAD_GATEWAY", framework::ResponseCodes::badGateway)
			.value("SERVICE_UNAVAILABLE", framework::ResponseCodes::serviceUnavailable)
			.value("GATEWAY_TIMEOUT", framework::ResponseCodes::gatewayTimeout)
			.value("HTTP_VERSION_NOT_SUPPORTED", framework::ResponseCodes::HTTPVersionNotSupported)
			.value("VARIANT_ALSO_NEGOTIATES", framework::ResponseCodes::variantAlsoNegotiates)
			.value("INSUFFICIENT_STORAGE", framework::ResponseCodes::insufficientStorage)
			.value("LOOP_DETECTED", framework::ResponseCodes::loopDetected)
			.value("BANDWIDTH_LIMIT_EXCEEDED", framework::ResponseCodes::bandwidthLimitExceeded)
			.value("NOT_EXTENDED", framework::ResponseCodes::notExtended)
			.value("NETWORK_AUTHENTICATION_REQUIRED", framework::ResponseCodes::networkAuthenticationRequired)
			.value("UNKNOWN_ERROR", framework::ResponseCodes::unknownError)
			.value("WEB_SERVER_IS_DOWN", framework::ResponseCodes::webServerIsDown)
			.value("CONNECTION_TIMED_OUT", framework::ResponseCodes::connectionTimedOut)
			.value("ORIGIN_IS_UNREACHABLE", framework::ResponseCodes::originIsUnreachable)
			.value("A_TIMEOUT_OCCURRED", framework::ResponseCodes::aTimeoutOccurred)
			.value("SSL_HANDSHAKE_FAILED", framework::ResponseCodes::SSLHandshakeFailed)
			.value("INVALID_SSL_CERTIFICATE", framework::ResponseCodes::invalidSSLCertificate)
			.export_values();

		executorSettings
			.def
			(
				py::init
				(
					[](uint64_t pointer)
					{
						return framework::utility::ExecutorSettings(reinterpret_cast<void*>(pointer));
					}
				),
				"pointer"_a
			)
			.def("get_name_type", &framework::utility::ExecutorSettings::getName)
			.def("get_user_agent_filter", &framework::utility::ExecutorSettings::getUserAgentFilter)
			.def("get_api_type", &framework::utility::ExecutorSettings::getAPIType)
			.def("get_load_type", &framework::utility::ExecutorSettings::getLoadType)
			.def
			(
				"get_init_parameters",
				[](const framework::utility::ExecutorSettings& self) -> py::dict
				{
					py::module_ json = py::module_::import("json");
					framework::JsonParser parser = self.getInitParameters();
					std::string_view data = *parser;

					return json.attr("loads")(data.data()).cast<py::dict>();
				}
			)
			.def
			(
				"register_dynamic_function",
				[](const framework::utility::ExecutorSettings& self, std::string_view functionName, py::type functionClass)
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
			.def("unregister_dynamic_function", &framework::utility::ExecutorSettings::unregisterDynamicFunction, "function_name"_a)
			.def("is_dynamic_function_registered", &framework::utility::ExecutorSettings::isDynamicFunctionRegistered, "function_name"_a)
			.def("get_file", &framework::utility::ExecutorSettings::getFile, "file_path"_a)
			.def("process_static_file", &framework::utility::ExecutorSettings::processStaticFile, "file_data"_a, "file_extension"_a)
			.def
			(
				"process_dynamic_file",
				[](const framework::utility::ExecutorSettings& self, std::string_view fileData, py::dict arguments)
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
				[](const framework::utility::ExecutorSettings& self, std::string_view databaseName, py::handle databaseImplementationClass)
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
				[](const framework::utility::ExecutorSettings& self, std::string_view databaseName, py::handle databaseImplementationClass)
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
				[](const framework::utility::ExecutorSettings& self, std::string_view databaseName, std::string_view tableName, std::string_view createTableQuery, py::handle databaseImplementationClass)
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
				[](const framework::utility::ExecutorSettings& self, std::string_view databaseName, std::string_view tableName, py::handle databaseImplementationClass)
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
	}

	void registerExecutors(pybind11::module_& m)
	{
		using namespace py::literals;

		py::class_<framework::StatelessExecutor, framework::PyStatelessExecutor>(m, "StatelessExecutor")
			.def(py::init())
			.def("init", &framework::StatelessExecutor::init, "settings"_a)
			.def("do_post", &framework::StatelessExecutor::doPost, "request"_a, "response"_a)
			.def("do_get", &framework::StatelessExecutor::doGet, "request"_a, "response"_a)
			.def("do_head", &framework::StatelessExecutor::doHead, "request"_a, "response"_a)
			.def("do_put", &framework::StatelessExecutor::doPut, "request"_a, "response"_a)
			.def("do_delete", &framework::StatelessExecutor::doDelete, "request"_a, "response"_a)
			.def("do_patch", &framework::StatelessExecutor::doPatch, "request"_a, "response"_a)
			.def("do_options", &framework::StatelessExecutor::doOptions, "request"_a, "response"_a)
			.def("do_trace", &framework::StatelessExecutor::doTrace, "request"_a, "response"_a)
			.def("do_connect", &framework::StatelessExecutor::doConnect, "request"_a, "response"_a)
			.def("get_type", &framework::StatelessExecutor::getType);

		py::class_<framework::StatefulExecutor, framework::PyStatefulExecutor>(m, "StatefulExecutor")
			.def(py::init())
			.def("init", &framework::StatefulExecutor::init, "settings"_a)
			.def("do_post", &framework::StatefulExecutor::doPost, "request"_a, "response"_a)
			.def("do_get", &framework::StatefulExecutor::doGet, "request"_a, "response"_a)
			.def("do_head", &framework::StatefulExecutor::doHead, "request"_a, "response"_a)
			.def("do_put", &framework::StatefulExecutor::doPut, "request"_a, "response"_a)
			.def("do_delete", &framework::StatefulExecutor::doDelete, "request"_a, "response"_a)
			.def("do_patch", &framework::StatefulExecutor::doPatch, "request"_a, "response"_a)
			.def("do_options", &framework::StatefulExecutor::doOptions, "request"_a, "response"_a)
			.def("do_trace", &framework::StatefulExecutor::doTrace, "request"_a, "response"_a)
			.def("do_connect", &framework::StatefulExecutor::doConnect, "request"_a, "response"_a)
			.def("get_type", &framework::StatefulExecutor::getType)
			.def("destroy", &framework::StatefulExecutor::destroy);

		py::class_<framework::HeavyOperationStatelessExecutor, framework::PyHeavyOperationStatelessExecutor>(m, "HeavyOperationStatelessExecutor")
			.def(py::init())
			.def("init", &framework::HeavyOperationStatelessExecutor::init, "settings"_a)
			.def("do_post", &framework::HeavyOperationStatelessExecutor::doPost, "request"_a, "response"_a)
			.def("do_get", &framework::HeavyOperationStatelessExecutor::doGet, "request"_a, "response"_a)
			.def("do_head", &framework::HeavyOperationStatelessExecutor::doHead, "request"_a, "response"_a)
			.def("do_put", &framework::HeavyOperationStatelessExecutor::doPut, "request"_a, "response"_a)
			.def("do_delete", &framework::HeavyOperationStatelessExecutor::doDelete, "request"_a, "response"_a)
			.def("do_patch", &framework::HeavyOperationStatelessExecutor::doPatch, "request"_a, "response"_a)
			.def("do_options", &framework::HeavyOperationStatelessExecutor::doOptions, "request"_a, "response"_a)
			.def("do_trace", &framework::HeavyOperationStatelessExecutor::doTrace, "request"_a, "response"_a)
			.def("do_connect", &framework::HeavyOperationStatelessExecutor::doConnect, "request"_a, "response"_a)
			.def("get_type", &framework::HeavyOperationStatelessExecutor::getType);

		py::class_<framework::HeavyOperationStatefulExecutor, framework::PyHeavyOperationStatefulExecutor>(m, "HeavyOperationStatefulExecutor")
			.def(py::init())
			.def("init", &framework::HeavyOperationStatefulExecutor::init, "settings"_a)
			.def("do_post", &framework::HeavyOperationStatefulExecutor::doPost, "request"_a, "response"_a)
			.def("do_get", &framework::HeavyOperationStatefulExecutor::doGet, "request"_a, "response"_a)
			.def("do_head", &framework::HeavyOperationStatefulExecutor::doHead, "request"_a, "response"_a)
			.def("do_put", &framework::HeavyOperationStatefulExecutor::doPut, "request"_a, "response"_a)
			.def("do_delete", &framework::HeavyOperationStatefulExecutor::doDelete, "request"_a, "response"_a)
			.def("do_patch", &framework::HeavyOperationStatefulExecutor::doPatch, "request"_a, "response"_a)
			.def("do_options", &framework::HeavyOperationStatefulExecutor::doOptions, "request"_a, "response"_a)
			.def("do_trace", &framework::HeavyOperationStatefulExecutor::doTrace, "request"_a, "response"_a)
			.def("do_connect", &framework::HeavyOperationStatefulExecutor::doConnect, "request"_a, "response"_a)
			.def("get_type", &framework::HeavyOperationStatefulExecutor::getType)
			.def("destroy", &framework::HeavyOperationStatefulExecutor::destroy);
	}
}
