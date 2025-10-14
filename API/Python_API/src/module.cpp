#define PYBIND11_DETAILED_ERROR_MESSAGES

#include <functional>
#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>
#include <pybind11/functional.h>

#include <WebFramework.hpp>

#include "Executors/PyStatelessExecutor.h"
#include "Executors/PyStatefulExecutor.h"
#include "Executors/PyHeavyOperationStatelessExecutor.h"
#include "Executors/PyHeavyOperationStatefulExecutor.h"

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
		.def("do_connect", &framework::BaseStatelessExecutor::doConnect, "request"_a, "response"_a);

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
		.def("do_connect", &framework::BaseHeavyOperationStatelessExecutor::doConnect, "request"_a, "response"_a);

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
		.def("destroy", &framework::BaseHeavyOperationStatefulExecutor::destroy);

	py::register_exception<framework::exceptions::WebFrameworkException>(m, "WebFrameworkException");
}
