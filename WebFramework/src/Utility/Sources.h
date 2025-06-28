#pragma once

#include "Framework/WebFrameworkPlatform.h"

#include "MultiLocalizationManager.h"

namespace framework::utility
{
	std::vector<HMODULE> loadSources(const std::vector<std::string>& pathToSources);

	template<typename T>
	inline T load(HMODULE handle, std::string_view name)
	{
		T function;

#ifdef __LINUX__
		function = reinterpret_cast<T>(dlsym(handle, name.data()));
#else
		function = reinterpret_cast<T>(GetProcAddress(handle, name.data()));
#endif

		if (!function)
		{
			throw std::runtime_error(std::format("Can't find {} function", name));
		}

		return function;
	};
}
