#include "Sources.h"

#include <Log.h>
#include <Exceptions/FileDoesNotExistException.h>

#include "DynamicLibraries.h"
#include "Exceptions/CantLoadSourceException.h"
#include "Framework/WebFrameworkConstants.h"

namespace framework::utility
{
	std::vector<std::pair<LoadSource, std::string>> loadSources(const std::vector<std::string>& pathToSources)
	{
		std::vector<std::pair<LoadSource, std::string>> result;

		if (pathToSources.size())
		{
			result.reserve(pathToSources.size());
		}
		else
		{
			result.emplace_back(nullptr, "");
		}

		for (const std::string& temp : pathToSources)
		{
			if (temp == json_settings_values::defaultLoadSourceValue)
			{
#ifdef __LINUX__
				result.emplace_back(dlopen(nullptr, RTLD_LAZY), "");
#else
				result.emplace_back(nullptr, "");
#endif

				continue;
			}

			utility::LoadSourceType type;
			std::string pathToSource = makePathToLoadSource(temp, type);
			std::string exceptionMessage;
			LoadSource source;

			if (type == utility::LoadSourceType::dynamicLibrary && !std::filesystem::exists(pathToSource))
			{
				if (Log::isValid())
				{
					Log::error("Can't find source {}", "LogWebFrameworkSources", pathToSource);
				}

				throw file_manager::exceptions::FileDoesNotExistException(pathToSource);
			}

			switch (type)
			{
			case framework::utility::LoadSourceType::dynamicLibrary:
#ifdef __LINUX__
				source = dlopen(pathToSource.data(), RTLD_LAZY);
#else
				source = LoadLibraryA(pathToSource.data());
#endif				
				if (!std::get<HMODULE>(source))
				{
					exceptionMessage = ::exceptions::missingOtherDLLs;
				}

				break;

			case framework::utility::LoadSourceType::python:
#ifdef __WITH_PYTHON_EXECUTORS__
				try
				{
					std::filesystem::path pythonSourcePath(pathToSource);
					py::module_ sys = py::module_::import("sys");

					sys.attr("path").attr("append")(pythonSourcePath.root_directory().string().data());
					
					source = py::module_::import(pythonSourcePath.filename().string().data());
				}
				catch (const py::error_already_set& e)
				{
					exceptionMessage = e.what();
				}
#else
				exceptionMessage = "Can't load Python Executor. WebFramework built without Python Executor support";
#endif
				
				break;

			default:
				throw std::runtime_error("Wrong LoadSourceType");
			}

			if (exceptionMessage.size())
			{
				if (Log::isValid())
				{
					Log::error("Can't load source {}, {}", "LogWebFrameworkSources", pathToSource, exceptionMessage);
				}

				throw exceptions::CantLoadSourceException(pathToSource, exceptionMessage);
			}

			result.emplace_back(std::move(source), pathToSource);
		}

		return result;
	}
}
