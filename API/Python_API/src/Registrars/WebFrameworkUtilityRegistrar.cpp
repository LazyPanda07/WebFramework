#include "Registrars/WebFrameworkUtilityRegistrar.h"

#include <cstdlib>

#include <pybind11/stl/filesystem.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

#include <Executors/Executor.hpp>
#include <Utility/WebFrameworkLocalization.hpp>
#include <Utility/WebFrameworkUtility.hpp>

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
			[](const std::filesystem::path& pathToDll)
			{
				if (pathToDll.empty())
				{
					static std::vector<HMODULE> libraries;
					constexpr std::array<std::string_view, 4> baseLibraryNames =
					{
						"FileManager",
						"Localization",
						"Log",
						"sqlite3"
					};

					auto finishLibraryName = [](std::string_view libraryName) -> std::string
						{
#ifdef __LINUX__
							return std::format("lib{}.so", libraryName);
#else
							return std::format("{}.dll", libraryName);
#endif
						};

					py::module_ os = py::module_::import("os");
					py::module_ webFrameworkApi = py::module_::import("web_framework_api");
					std::filesystem::path basePath = os.attr("path").attr("dirname")(webFrameworkApi.attr("__file__")).cast<std::string>();

					for (std::string_view libraryName : baseLibraryNames)
					{
						std::filesystem::path actualPath = basePath / finishLibraryName(libraryName);

#ifdef __LINUX__
						libraries.emplace_back(dlopen(actualPath.string().data(), RTLD_NOW | RTLD_GLOBAL));
#else
						libraries.emplace_back(LoadLibraryA(actualPath.string().data()));
#endif
					}

#ifdef __LINUX__
					framework::utility::initializeWebFramework(basePath / "libWebFramework.so");
#else
					framework::utility::initializeWebFramework(basePath / "WebFramework.dll");
#endif
				}
				else
				{
					framework::utility::initializeWebFramework(pathToDll);
				}
			},
			"path_to_dll"_a = ""
		);

		m.def("get_localized_string", &framework::utility::getLocalizedString, "localization_module_name"_a, "key"_a, "language"_a = "");

		m.def
		(
			"generate_binary_asset_file",
			[](const std::filesystem::path& directoryPath, const std::filesystem::path& outputPath, const std::optional<std::function<void(float progress, std::string_view assetPath, py::object data)>>& progressCallback, const std::optional<py::object>& data)
			{
				auto callback = [](float progress, const char* assetPath, void* data)
					{
						void** pack = reinterpret_cast<void**>(data);

						if (pack[0])
						{
							std::function<void(float progress, std::string_view assetPath, py::object data)>& actualCallback = *reinterpret_cast<std::function<void(float progress, std::string_view assetPath, py::object data)>*>(pack[0]);
							py::object* actualData = nullptr;

							if (pack[1])
							{
								actualData = reinterpret_cast<py::object*>(pack[1]);
							}

							actualCallback(progress, assetPath, actualData ? *actualData : py::none());
						}
					};
				const void* pack[2]{};

				pack[0] = nullptr;
				pack[1] = nullptr;

				if (progressCallback)
				{
					pack[0] = &*progressCallback;
				}

				if (data)
				{
					pack[1] = &*data;
				}

				framework::utility::generateBinaryAssetFile
				(
					directoryPath,
					outputPath,
					callback,
					pack
				);

			},
			"directory_path"_a, "output_path"_a, "progress_callback"_a.noconvert() = std::nullopt, "data"_a = std::nullopt
		);

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

		m.def
		(
			"create_jwt",
			[](py::dict data, int64_t expirationTimeInMinutes, std::string_view jwtSecretVariableName)
			{
				py::module json = py::module::import("json");
				framework::JsonParser parser(json.attr("dumps")(data).cast<std::string>());

				return framework::utility::token::createJwt(parser.getParsedData(), std::chrono::minutes(expirationTimeInMinutes), jwtSecretVariableName);
			},
			"data"_a, "expiration_time_in_minutes"_a, "jwt_secret_variable_name"_a = "JWT_SECRET"
		);

		m.def
		(
			"create_jwt",
			[](py::dict data, int64_t expirationTimeInMinutes, const framework::WebFramework& frameworkInstance)
			{
				py::module json = py::module::import("json");
				framework::JsonParser parser(json.attr("dumps")(data).cast<std::string>());

				return framework::utility::token::createJwt(parser.getParsedData(), std::chrono::minutes(expirationTimeInMinutes), frameworkInstance);
			},
			"data"_a, "expiration_time_in_minutes"_a, "framework_instance"_a
		);

		py::register_exception<framework::exceptions::WebFrameworkException>(m, "WebFrameworkException");
	}

	void registerUtilityStructures(pybind11::module_& m)
	{
		using namespace py::literals;

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
			.def("__call__", &framework::IDynamicFunction::operator(), "arguments"_a);

		py::class_<framework::utility::IPyChunkGenerator, framework::utility::PyChunkGenerator>(m, "ChunkGenerator")
			.def(py::init())
			.def("generate", &framework::utility::IPyChunkGenerator::generate, py::doc("generate() -> str | bytes"));

		py::class_<framework::interfaces::IHttpRequest>(m, "IHttpRequest").doc() = "Forward declaration. Internal use only";
		py::class_<framework::interfaces::IHttpResponse>(m, "IHttpResponse").doc() = "Forward declaration. Internal use only";
	}
}
