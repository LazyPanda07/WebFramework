#define PYBIND11_DETAILED_ERROR_MESSAGES

#include <functional>
#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>
#include <pybind11/functional.h>

#include <WebFramework.hpp>

#include "Executors/PyBaseStatelessExecutor.h"

namespace py = pybind11;

PYBIND11_MODULE(web_framework_api, m, py::mod_gil_not_used())
{
	using namespace py::literals;

	m.doc() = "Python API for WebFramework";

	m.def("initialize_web_framework", &framework::utility::initializeWebFramework, "path_to_dll"_a = "");

	py::class_<framework::utility::ExecutorSettings>(m, "ExecutorSettings")
		.def("get_api_type", &framework::utility::ExecutorSettings::getAPIType);

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
		.def("add_header", &framework::HTTPResponse::addHeader, "name"_a, "value"_a);

	py::class_<framework::BaseStatelessExecutor, framework::PyBaseStatelessExecutor>(m, "BaseStatelessExecutor")
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
		.def("do_connect", &framework::BaseStatelessExecutor::doConnect, "request"_a, "response"_a);

	py::register_exception<framework::exceptions::WebFrameworkException>(m, "WebFrameworkException");
}
