#pragma once

#include "Framework/WebFrameworkPlatform.h"

#include <MultiLocalizationManager.h>

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
		>;

	std::vector<std::pair<LoadSource, std::string>> loadSources(const std::vector<std::string>& pathToSources);

	template<typename T>
	inline T load(HMODULE handle, std::string_view name)
	{
#ifdef __LINUX__
		return reinterpret_cast<T>(dlsym(handle, name.data()));
#else
		return reinterpret_cast<T>(GetProcAddress(handle, name.data()));
#endif
	};
}
