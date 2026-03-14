#include "Registrars/WebFrameworkApiRegistrar.h"

#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

#include <import.hpp>

namespace py = pybind11;

namespace registrar
{
	void registerWebFrameworkApi(pybind11::module_& m)
	{
		using namespace py::literals;

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
			.def("kick", &framework::WebFramework::kick, "ip"_a)
			.def("update_ssl_certificates", &framework::WebFramework::updateSslCertificates)
			.def("is_server_running", &framework::WebFramework::isServerRunning)
			.def_static("get_web_framework_version", &framework::WebFramework::getWebFrameworkVersion);
	}
}
