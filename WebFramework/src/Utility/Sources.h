#pragma once

#include "core.h"

#include "MultiLocalizationManager.h"

namespace framework
{
    namespace utility
    {
        std::vector<HMODULE> loadSources(const std::vector<std::string>& pathToSources);

        inline auto load(HMODULE handle, std::string_view name)
		{
#ifdef __LINUX__
			return dlsym(handle, name.data());
#else
			return GetProcAddress(handle, name.data());
#endif
	    };
    }
}
