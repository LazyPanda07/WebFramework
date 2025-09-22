#pragma once

#include "Framework/WebFrameworkPlatform.h"

#include <MultiLocalizationManager.h>

namespace framework::utility
{
	std::vector<std::pair<HMODULE, std::string>> loadSources(const std::vector<std::string>& pathToSources);

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
