#define PYBIND11_DETAILED_ERROR_MESSAGES

#include <functional>
#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>
#include <pybind11/functional.h>

#include <WebFramework.hpp>
#include <Utility/WebFrameworkLocalization.hpp>
#include <Utility/WebFrameworkUtility.hpp>

#include "Executors/PyStatelessExecutor.h"
#include "Executors/PyStatefulExecutor.h"
#include "Executors/PyHeavyOperationStatelessExecutor.h"
#include "Executors/PyHeavyOperationStatefulExecutor.h"
#include "PyChunkGenerator.h"
#include "PyLoadBalancerHeuristic.h"

namespace py = pybind11;

PYBIND11_MODULE(web_framework_api, m, py::mod_gil_not_used())
{
	using namespace py::literals;

	m.doc() = "Python API for WebFramework";

	m.def("initialize_web_framework", &framework::utility::initializeWebFramework, "path_to_dll"_a = "");
	m.def("get_localized_string", &framework::utility::getLocalizedString, "localization_module_name"_a, "key"_a, "language"_a = "");
	m.def("generate_uuid", &framework::utility::uuid::generateUUID);

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
				framework::JSONParser parser = self.getInitParameters();
				std::string_view data = *parser;

				return json.attr("loads")(data.data()).cast<py::dict>();
			}
		);

	py::class_<framework::Multipart>(m, "Multipart")
		.def("get_name", &framework::Multipart::getName)
		.def("get_content_type", &framework::Multipart::getContentType)
		.def("get_data", &framework::Multipart::getData)
		.def("get_file_name", &framework::Multipart::getFileName);

	py::class_<framework::LargeData>(m, "LargeData")
		.def("__iter__", [](const framework::LargeData& self) { return py::iter(py::make_tuple(py::bytes(self.dataPart), self.isLastPacket)); })
		.def("__len__", [](const framework::LargeData& self) { return 2; });

	py::class_<framework::utility::Config>(m, "Config")
		.def(py::init<const std::filesystem::path&>(), "config_path"_a)
		.def(py::init<std::string_view, std::string_view>(), "server_configuration"_a, "application_directory"_a)
		.def(py::init<const std::filesystem::path&>(), "config_path"_a)
		.def
		(
			"override_configuration",
			py::overload_cast<std::string_view, const std::string&, bool>
			(
				static_cast<framework::utility::Config & (framework::utility::Config::*)(std::string_view, const std::string&, bool)>(&framework::utility::Config::overrideConfiguration<std::string>)
			),
			"key"_a, "value"_a, "recursive"_a = true
		)
		.def
		(
			"override_configuration",
			py::overload_cast<std::string_view, const bool&, bool>
			(
				static_cast<framework::utility::Config & (framework::utility::Config::*)(std::string_view, const bool&, bool)>(&framework::utility::Config::overrideConfiguration<bool>)
			),
			"key"_a, "value"_a, "recursive"_a = true
		)
		.def
		(
			"override_configuration",
			py::overload_cast<std::string_view, const std::int64_t&, bool>
			(
				static_cast<framework::utility::Config & (framework::utility::Config::*)(std::string_view, const std::int64_t&, bool)>(&framework::utility::Config::overrideConfiguration<std::int64_t>)
			),
			"key"_a, "value"_a, "recursive"_a = true
		)
		.def
		(
			"override_configuration",
			py::overload_cast<std::string_view, const std::vector<std::string>&, bool>
			(
				static_cast<framework::utility::Config & (framework::utility::Config::*)(std::string_view, const std::vector<std::string>&, bool)>(&framework::utility::Config::overrideConfiguration)
			),
			"key"_a, "value"_a, "recursive"_a = true
		)
		.def
		(
			"override_configuration",
			py::overload_cast<std::string_view, const std::vector<std::int64_t>&, bool>
			(
				static_cast<framework::utility::Config & (framework::utility::Config::*)(std::string_view, const std::vector<std::int64_t>&, bool)>(&framework::utility::Config::overrideConfiguration)
			),
			"key"_a, "value"_a, "recursive"_a = true
		)
		.def("override_base_path", &framework::utility::Config::overrideBasePath, "base_path"_a)
		.def("get_configuration_string", &framework::utility::Config::get<std::string>, "key"_a, "recursive"_a = true)
		.def("get_configuration_int", &framework::utility::Config::get<int64_t>, "key"_a, "recursive"_a = true)
		.def("get_configuration_bool", &framework::utility::Config::get<bool>, "key"_a, "recursive"_a = true)
		.def("get_base_path", &framework::utility::Config::getBasePath)
		.def("get_configuration", &framework::utility::Config::getConfiguration)
		.def("get_raw_configuration", &framework::utility::Config::getRawConfiguration);

	py::class_<framework::WebFramework>(m, "WebFramework")
		.def(py::init<std::string_view>(), "config_path"_a)
		.def(py::init<std::string_view, std::string_view>(), "server_configuration"_a, "application_directory"_a)
		.def(py::init<const framework::utility::Config&>(), "config"_a)
		.def("start", &framework::WebFramework::start, "wait"_a = false, "on_start_server"_a = py::none(), py::call_guard<py::gil_scoped_release>())
		.def("stop", &framework::WebFramework::stop, "wait"_a = true)
		.def("is_server_running", &framework::WebFramework::isServerRunning)
		.def_static("get_web_framework_version", &framework::WebFramework::getWebFrameworkVersion);

	py::class_<framework::interfaces::IHTTPRequest> request(m, "IHTTPRequest");
	py::class_<framework::interfaces::IHTTPResponse> response(m, "IHTTPResponse");

	py::class_<framework::SQLValue>(m, "SQLValue")
		.def(py::init<int64_t>(), "value"_a)
		.def(py::init<double>(), "value"_a)
		.def(py::init<const std::string&>(), "value"_a)
		.def(py::init<std::nullptr_t>(), "value"_a)
		.def(py::init<const std::vector<uint8_t>&>(), "value"_a)
		.def("get", &framework::SQLValue::operator*);

	py::class_<framework::SQLResult>(m, "SQLResult")
		.def("at", [](const framework::SQLResult& self, size_t index) { return self.at(index); }, "index"_a)
		.def("__len__", &framework::SQLResult::size)
		.def("__iter__", [](framework::SQLResult& self) { return py::make_iterator(self.begin(), self.end()); }, py::keep_alive<0, 1>())
		.def("__getitem__", [](const framework::SQLResult& self, size_t index) { return self[index]; }, "index"_a);

	py::class_<framework::Table>(m, "Table")
		.def("execute", [](framework::Table& table, std::string_view query, const std::vector<framework::SQLValue>& values = {}) { return table.execute(query, values); }, "query"_a, "values"_a = py::list());

	py::class_<framework::Database>(m, "Database")
		.def("__contains__", [](const framework::Database& self, std::string_view tableName) { return self.contains(tableName); }, "table_name"_a)
		.def("get_table", &framework::Database::getTable, "table_name"_a)
		.def("get_or_create_table", &framework::Database::getOrCreateTable, "table_name"_a, "create_table_query"_a)
		.def("get_database_name", &framework::Database::getDatabaseName)
		.def("get_database_file_name", &framework::Database::getDatabaseFileName)
		.def("__getitem__", &framework::Database::getTable, "table_name"_a)
		.def("__contains__", [](const framework::Database& self, std::string_view tableName) { return self.contains(tableName); }, "table_name"_a);

	m.def
	(
		"make_sql_values",
		[](py::args args) -> std::vector<framework::SQLValue>
		{
			std::vector<framework::SQLValue> result;

			result.reserve(args.size());

			for (py::handle item : args)
			{
				if (py::isinstance<py::str>(item))
				{
					result.emplace_back(item.cast<std::string>());
				}
				else if (py::isinstance<py::bool_>(item))
				{
					result.emplace_back(item.cast<bool>());
				}
				else if (py::isinstance<py::int_>(item))
				{
					result.emplace_back(item.cast<int64_t>());
				}
				else if (py::isinstance<py::float_>(item))
				{
					result.emplace_back(item.cast<double>());
				}
				else if (item.is_none())
				{
					result.emplace_back(nullptr);
				}
				else if (py::isinstance<py::bytes>(item))
				{
					py::bytes bytes = item.cast<py::bytes>();
					std::string temp(bytes);

					result.emplace_back(std::vector<uint8_t>(temp.begin(), temp.end()));
				}
				else if (py::isinstance<py::bytearray>(item))
				{
					py::bytearray bytes = item.cast<py::bytearray>();
					std::string temp(bytes);

					result.emplace_back(std::vector<uint8_t>(temp.begin(), temp.end()));
				}
				else
				{
					throw std::runtime_error("Unsupported type: " + py::repr(item.get_type()).cast<std::string>());
				}
			}

			return result;
		}
	);

	py::class_<framework::HTTPResponse>(m, "HTTPResponse")
		.def
		(
			py::init
			(
				[](uint64_t pointer)
				{
					return framework::HTTPResponse(reinterpret_cast<framework::interfaces::IHTTPResponse*>(pointer));
				}
			),
			"pointer_a"
		)
		.def("set_body", py::overload_cast<std::string_view>(&framework::HTTPResponse::setBody), "body"_a)
		.def
		(
			"set_body",
			[](framework::HTTPResponse& self, const py::dict& json)
			{
				py::module_ jsonModule = py::module_::import("json");

				self.setBody(framework::JSONBuilder(jsonModule.attr("dumps")(json).cast<std::string>()));
			},
			"json"_a
		)
		.def("append_body", &framework::HTTPResponse::appendBody, "body"_a)
		.def("add_cookie", &framework::HTTPResponse::addCookie, "name"_a, "value"_a)
		.def("add_header", &framework::HTTPResponse::addHeader, "name"_a, "value"_a)
		.def("set_default", &framework::HTTPResponse::setDefault)
		.def("set_is_valid", &framework::HTTPResponse::setIsValid)
		.def("set_response_code", &framework::HTTPResponse::setResponseCode, "response_code"_a)
		.def("__bool__", [](const framework::HTTPResponse& self) { return static_cast<bool>(self); });

	py::class_<framework::utility::ChunkGenerator, framework::utility::PyChunkGenerator>(m, "ChunkGenerator")
		.def(py::init())
		.def("generate", &framework::utility::ChunkGenerator::generate);

	py::class_<framework::LoadBalancerHeuristic, framework::PyLoadBalancerHeuristic>(m, "LoadBalancerHeuristic")
		.def(py::init<std::string_view, std::string_view, bool>(), "ip"_a, "port"_a, "use_https"_a)
		.def("get_ip", &framework::LoadBalancerHeuristic::getIp)
		.def("get_port", &framework::LoadBalancerHeuristic::getPort)
		.def("get_use_https", &framework::LoadBalancerHeuristic::getUseHTTPS)
		.def("on_start", &framework::LoadBalancerHeuristic::onStart)
		.def("on_end", &framework::LoadBalancerHeuristic::onEnd)
		.def("__call__", &framework::LoadBalancerHeuristic::operator());

	py::class_<framework::HTTPRequest>(m, "HTTPRequest")
		.def
		(
			py::init
			(
				[](uint64_t pointer)
				{
					return framework::HTTPRequest(reinterpret_cast<framework::interfaces::IHTTPRequest*>(pointer));
				}
			),
			"pointer"_a
		)
		.def("get_raw_parameters", &framework::HTTPRequest::getRawParameters)
		.def("get_query_parameters", &framework::HTTPRequest::getQueryParameters)
		.def("get_http_version", &framework::HTTPRequest::getHTTPVersion)
		.def("get_headers", &framework::HTTPRequest::getHeaders)
		.def("get_body", &framework::HTTPRequest::getBody)
		.def("set_attribute", &framework::HTTPRequest::setAttribute, "name"_a, "value"_a)
		.def("get_attribute", &framework::HTTPRequest::setAttribute)
		.def("delete_session", &framework::HTTPRequest::deleteSession)
		.def("remove_attribute", &framework::HTTPRequest::removeAttribute, "name"_a)
		.def("get_cookies", &framework::HTTPRequest::getCookies)
		.def("get_multiparts", &framework::HTTPRequest::getMultiparts)
		.def("get_large_data", &framework::HTTPRequest::getLargeData)
		.def("send_asset_file", &framework::HTTPRequest::sendAssetFile, "file_path"_a, "response"_a, "variables"_a, "is_binary"_a, "file_name"_a)
		.def("send_static_file", &framework::HTTPRequest::sendStaticFile, "file_path"_a, "response"_a, "is_binary"_a, "file_name"_a)
		.def("send_wfdp_file", &framework::HTTPRequest::sendWFDPFile, "file_path"_a, "response"_a, "variables"_a, "is_binary"_a = false, "file_name"_a = "")
		.def("stream_file", &framework::HTTPRequest::streamFile, "file_path"_a, "response"_a, "file_name"_a, "chunk_size"_a = framework::interfaces::IHTTPRequest::defaultChunkSize)
		/*.def
		(
			"register_wfdp_function",
			[](framework::HTTPRequest& self, std::string_view functionName, const std::function<const char*(const char**, size_t)>& function)
			{
				auto inner = [](const char** args, size_t size) -> const char*
					{
						return nullptr;
					};
				auto deleter = [](char* result)
					{

					};

				self.registerWFDPFunction(functionName, inner, deleter);
			},
			"function_name"_a, "function"_a
		)*/
		.def("unregister_wfdp_function", &framework::HTTPRequest::unregisterWFDPFunction, "function_name"_a)
		.def("is_wfdp_function_registered", &framework::HTTPRequest::isWFDPFunctionRegistered, "function_name"_a)
		.def("get_file", &framework::HTTPRequest::getFile, "file_path"_a)
		.def("process_static_file", &framework::HTTPRequest::processStaticFile, "file_data"_a, "file_extension"_a)
		.def("process_wfdp_file", &framework::HTTPRequest::processWFDPFile, "file_data"_a, "variables"_a)
		.def
		(
			"get_json",
			[](const framework::HTTPRequest& self)
			{
				py::module_ json = py::module_::import("json");
				const framework::JSONParser& parser = self.getJSON();

				return json.attr("loads")((*parser).data()).cast<py::dict>();
			}
		)
		.def("get_chunks", &framework::HTTPRequest::getChunks)
		.def("get_raw_request", &framework::HTTPRequest::getRawRequest)
		.def("get_client_ip_v4", &framework::HTTPRequest::getClientIpV4)
		.def("get_server_ip_v4", &framework::HTTPRequest::getServerIpV4)
		.def("get_client_port", &framework::HTTPRequest::getClientPort)
		.def("get_server_port", &framework::HTTPRequest::getServerPort)
		.def("get_or_create_database", &framework::HTTPRequest::getOrCreateDatabase, "database_name"_a)
		.def("get_database", &framework::HTTPRequest::getDatabase, "database_name"_a)
		.def("get_or_create_table", &framework::HTTPRequest::getOrCreateTable, "database_name"_a, "table_name"_a, "create_table_query"_a)
		.def("get_table", &framework::HTTPRequest::getTable, "database_name"_a, "table_name"_a)
		.def("get_route_parameter", &framework::HTTPRequest::getRouteParameter<std::string>, "route_parameter_name"_a)
		.def("get_route_parameter", &framework::HTTPRequest::getRouteParameter<bool>, "route_parameter_name"_a)
		.def("get_route_parameter", &framework::HTTPRequest::getRouteParameter<int64_t>, "route_parameter_name"_a)
		.def("get_route_parameter", &framework::HTTPRequest::getRouteParameter<double>, "route_parameter_name"_a)
		.def
		(
			"throw_exception",
			[](framework::HTTPRequest& self, std::string_view errorMessage, framework::ResponseCodes responseCode, std::string_view logCategory = "") { self.throwException(errorMessage, responseCode, logCategory); },
			"error_message"_a, "response_code"_a, "log_category"_a = ""
		)
		.def("send_chunks", [](framework::HTTPRequest& self, framework::HTTPResponse& response, framework::utility::PyChunkGenerator& generator) { self.sendChunks(response, generator); }, "response"_a, "generator"_a)
		.def
		(
			"send_file_chunks",
			[](framework::HTTPRequest& self, framework::HTTPResponse& response, std::string_view fileName, framework::utility::PyChunkGenerator& generator) { self.sendFileChunks(response, fileName, generator); },
			"response"_a, "file_name"_a, "generator"_a
		)
		.def("get_method", &framework::HTTPRequest::getMethod);

	py::class_<framework::BaseStatelessExecutor, framework::PyStatelessExecutor>(m, "StatelessExecutor")
		.def(py::init())
		.def("init", &framework::BaseStatelessExecutor::init, "settings"_a)
		.def("do_post", &framework::BaseStatelessExecutor::doPost, "request"_a, "response"_a)
		.def("do_get", &framework::BaseStatelessExecutor::doGet, "request"_a, "response"_a)
		.def("do_head", &framework::BaseStatelessExecutor::doHead, "request"_a, "response"_a)
		.def("do_put", &framework::BaseStatelessExecutor::doPut, "request"_a, "response"_a)
		.def("do_delete", &framework::BaseStatelessExecutor::doDelete, "request"_a, "response"_a)
		.def("do_patch", &framework::BaseStatelessExecutor::doPatch, "request"_a, "response"_a)
		.def("do_options", &framework::BaseStatelessExecutor::doOptions, "request"_a, "response"_a)
		.def("do_trace", &framework::BaseStatelessExecutor::doTrace, "request"_a, "response"_a)
		.def("do_connect", &framework::BaseStatelessExecutor::doConnect, "request"_a, "response"_a)
		.def("get_type", &framework::BaseStatelessExecutor::getType);

	py::class_<framework::BaseStatefulExecutor, framework::PyStatefulExecutor>(m, "StatefulExecutor")
		.def(py::init())
		.def("init", &framework::BaseStatefulExecutor::init, "settings"_a)
		.def("do_post", &framework::BaseStatefulExecutor::doPost, "request"_a, "response"_a)
		.def("do_get", &framework::BaseStatefulExecutor::doGet, "request"_a, "response"_a)
		.def("do_head", &framework::BaseStatefulExecutor::doHead, "request"_a, "response"_a)
		.def("do_put", &framework::BaseStatefulExecutor::doPut, "request"_a, "response"_a)
		.def("do_delete", &framework::BaseStatefulExecutor::doDelete, "request"_a, "response"_a)
		.def("do_patch", &framework::BaseStatefulExecutor::doPatch, "request"_a, "response"_a)
		.def("do_options", &framework::BaseStatefulExecutor::doOptions, "request"_a, "response"_a)
		.def("do_trace", &framework::BaseStatefulExecutor::doTrace, "request"_a, "response"_a)
		.def("do_connect", &framework::BaseStatefulExecutor::doConnect, "request"_a, "response"_a)
		.def("get_type", &framework::BaseStatefulExecutor::getType)
		.def("destroy", &framework::BaseStatefulExecutor::destroy);

	py::class_<framework::BaseHeavyOperationStatelessExecutor, framework::PyHeavyOperationStatelessExecutor>(m, "HeavyOperationStatelessExecutor")
		.def(py::init())
		.def("init", &framework::BaseHeavyOperationStatelessExecutor::init, "settings"_a)
		.def("do_post", &framework::BaseHeavyOperationStatelessExecutor::doPost, "request"_a, "response"_a)
		.def("do_get", &framework::BaseHeavyOperationStatelessExecutor::doGet, "request"_a, "response"_a)
		.def("do_head", &framework::BaseHeavyOperationStatelessExecutor::doHead, "request"_a, "response"_a)
		.def("do_put", &framework::BaseHeavyOperationStatelessExecutor::doPut, "request"_a, "response"_a)
		.def("do_delete", &framework::BaseHeavyOperationStatelessExecutor::doDelete, "request"_a, "response"_a)
		.def("do_patch", &framework::BaseHeavyOperationStatelessExecutor::doPatch, "request"_a, "response"_a)
		.def("do_options", &framework::BaseHeavyOperationStatelessExecutor::doOptions, "request"_a, "response"_a)
		.def("do_trace", &framework::BaseHeavyOperationStatelessExecutor::doTrace, "request"_a, "response"_a)
		.def("do_connect", &framework::BaseHeavyOperationStatelessExecutor::doConnect, "request"_a, "response"_a)
		.def("get_type", &framework::BaseHeavyOperationStatelessExecutor::getType);

	py::class_<framework::BaseHeavyOperationStatefulExecutor, framework::PyHeavyOperationStatefulExecutor>(m, "HeavyOperationStatefulExecutor")
		.def(py::init())
		.def("init", &framework::BaseHeavyOperationStatefulExecutor::init, "settings"_a)
		.def("do_post", &framework::BaseHeavyOperationStatefulExecutor::doPost, "request"_a, "response"_a)
		.def("do_get", &framework::BaseHeavyOperationStatefulExecutor::doGet, "request"_a, "response"_a)
		.def("do_head", &framework::BaseHeavyOperationStatefulExecutor::doHead, "request"_a, "response"_a)
		.def("do_put", &framework::BaseHeavyOperationStatefulExecutor::doPut, "request"_a, "response"_a)
		.def("do_delete", &framework::BaseHeavyOperationStatefulExecutor::doDelete, "request"_a, "response"_a)
		.def("do_patch", &framework::BaseHeavyOperationStatefulExecutor::doPatch, "request"_a, "response"_a)
		.def("do_options", &framework::BaseHeavyOperationStatefulExecutor::doOptions, "request"_a, "response"_a)
		.def("do_trace", &framework::BaseHeavyOperationStatefulExecutor::doTrace, "request"_a, "response"_a)
		.def("do_connect", &framework::BaseHeavyOperationStatefulExecutor::doConnect, "request"_a, "response"_a)
		.def("get_type", &framework::BaseHeavyOperationStatefulExecutor::getType)
		.def("destroy", &framework::BaseHeavyOperationStatefulExecutor::destroy);

	py::register_exception<framework::exceptions::WebFrameworkException>(m, "WebFrameworkException");
}
