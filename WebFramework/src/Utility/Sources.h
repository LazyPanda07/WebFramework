#pragma once

#include "Framework/WebFrameworkPlatform.h"

#include <MultiLocalizationManager.h>
#include <Log.h>

#ifdef __WITH_PYTHON_EXECUTORS__
#include <pybind11/embed.h>

namespace py = pybind11;
#endif

namespace framework::utility
{
	using LoadSource = std::variant
		<
		HMODULE
#ifdef __WITH_PYTHON_EXECUTORS__
		, py::module_
#endif
#ifdef __WITH_DOT_NET_EXECUTORS__
		, std::filesystem::path // absolute path to .NET module
#endif
		>;

	std::vector<std::pair<LoadSource, std::string>> loadSources(const std::vector<std::string>& pathToSources);

	template<typename T>
	inline T load(HMODULE handle, std::string_view name, bool throwException = false)
	{
		T result;

#ifdef __LINUX__
		result = reinterpret_cast<T>(dlsym(handle, name.data()));
#else
		result = reinterpret_cast<T>(GetProcAddress(handle, name.data()));
#endif

		if (throwException && !result)
		{
			std::string errorMessage = std::format("Can't load: {} function", name);

			if (Log::isValid())
			{
				Log::error(errorMessage, "LogLoad");
			}

			throw std::runtime_error(errorMessage);
		}

		return result;
	};
}
