#pragma once

#include "Framework/WebFrameworkPlatform.h"

#include <MultiLocalizationManager.h>

#ifdef __WITH_PYTHON_EXECUTORS__
#include <pybind11/embed.h>

namespace py = pybind11;
#endif

#include "Utility/Utils.h"

namespace framework::utility
{
	using LoadSource = std::variant
		<
		HMODULE
#ifdef __WITH_PYTHON_EXECUTORS__
		, py::module_
#endif
#ifdef __WITH_DOTNET_EXECUTORS__
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
			utility::logAndThrowException<logging::message::cantLoadFunction, logging::category::loadSource>(name);
		}

		return result;
	};
}
