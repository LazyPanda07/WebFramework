#include "Registrars/WebFrameworkUtilityRegistrar.h"

#include <cstdlib>

#include <pybind11/stl/filesystem.h>
#include <pybind11/stl.h>

#include <Executors/Executor.hpp>
#include <Utility/WebFrameworkLocalization.hpp>

#include "PyDynamicFunction.h"
#include "PyChunkGenerator.h"

namespace py = pybind11;

namespace registrar
{
	void registerUtility(pybind11::module_& m)
	{
		using namespace py::literals;

		m.def
		(
			"initialize_web_framework",
			[](std::string_view pathToDLL)
			{
				py::module_ os = py::module_::import("os");
				py::module_ sys = py::module_::import("sys");
				std::string separator = os.attr("pathsep").cast<std::string>();
				py::list dirs = sys.attr("path").cast<py::list>();
				std::string path = std::getenv("PATH");

				for (py::handle temp : dirs)
				{
					path = std::format("{}{}{}", temp.cast<std::string>(), separator, path);
				}

#ifdef __LINUX__
				putenv(std::format("LD_LIBRARY_PATH={}", path).data());
#else
				_putenv_s("PATH", path.data());
#endif

				framework::utility::initializeWebFramework(pathToDLL);
			},
			"path_to_dll"_a = ""
		);

		m.def("get_localized_string", &framework::utility::getLocalizedString, "localization_module_name"_a, "key"_a, "language"_a = "");

		m.def
		(
			"make_sql_values",
			[](py::args args) -> std::vector<framework::SqlValue>
			{
				std::vector<framework::SqlValue> result;

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
						throw std::runtime_error("Unsupported type: " + py::repr(py::type::of(item)).cast<std::string>());
					}
				}

				return result;
			}
		);

		py::register_exception<framework::exceptions::WebFrameworkException>(m, "WebFrameworkException");
	}

	void registerUtilityStructures(pybind11::module_& m)
	{
		py::class_<framework::Multipart>(m, "Multipart")
			.def("get_name", &framework::Multipart::getName)
			.def("get_content_type", &framework::Multipart::getContentType)
			.def("get_data", &framework::Multipart::getData)
			.def("get_file_name", &framework::Multipart::getFileName);

		py::class_<framework::LargeData>(m, "LargeData")
			.def("__iter__", [](const framework::LargeData& self) { return py::iter(py::make_tuple(py::bytes(self.dataPart), self.isLastPacket)); })
			.def("__len__", [](const framework::LargeData& self) { return 2; });

		py::class_<framework::IDynamicFunction, framework::PyDynamicFunction>(m, "DynamicFunction")
			.def(py::init())
			.def("__call__", &framework::IDynamicFunction::operator());

		py::class_<framework::utility::IPyChunkGenerator, framework::utility::PyChunkGenerator>(m, "ChunkGenerator")
			.def(py::init())
			.def("generate", &framework::utility::IPyChunkGenerator::generate, py::doc("generate() -> str | bytes"));

		py::class_<framework::interfaces::IHttpRequest>(m, "IHttpRequest").doc() = "Forward declaration. Internal use only";
		py::class_<framework::interfaces::IHttpResponse>(m, "IHttpResponse").doc() = "Forward declaration. Internal use only";
	}
}
