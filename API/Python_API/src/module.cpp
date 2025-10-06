#define PYBIND11_DETAILED_ERROR_MESSAGES

#include <functional>
#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "WebFramework.hpp"

namespace py = pybind11;

PYBIND11_MODULE(web_framework_api, m, py::mod_gil_not_used())
{
	using namespace py::literals;

	m.doc() = "Python API for WebFramework";

	m.def("initialize_web_framework", &framework::utility::initializeWebFramework, "path_to_dll"_a = "");

	py::class_<framework::WebFramework>(m, "WebFramework")
		.def(py::init<std::string_view>(), "config_path"_a)
		.def(py::init<std::string_view, std::string_view>(), "server_configuration"_a, "application_directory"_a)
		.def(py::init<const framework::utility::Config&>(), "config"_a)
		.def("start", &framework::WebFramework::start, "wait"_a = false, "on_start_server"_a = py::none())
		.def("stop", &framework::WebFramework::stop, "wait"_a = true)
		.def("is_server_running", &framework::WebFramework::isServerRunning)
		.def_static("get_web_framework_version", &framework::WebFramework::getWebFrameworkVersion);

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
			py::overload_cast<std::string_view, const std::int64_t&, bool>
			(
				static_cast<framework::utility::Config & (framework::utility::Config::*)(std::string_view, const std::int64_t&, bool)>(&framework::utility::Config::overrideConfiguration<std::int64_t>)
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

	py::register_exception<framework::exceptions::WebFrameworkException>(m, "WebFrameworkException");
}
