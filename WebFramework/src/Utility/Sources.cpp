#include "Sources.h"

using namespace std;

namespace framework
{
    namespace utility
    {
        vector<HMODULE> loadSources(const vector<string>& pathToSources)
        {
            vector<HMODULE> result;

			result.reserve(pathToSources.size());

			for (const string& temp : pathToSources)
			{
				if (temp == json_settings::defaultLoadSourceValue)
				{
#ifdef __LINUX__
					result.push_back(dlopen(nullptr, RTLD_LAZY));
#else
					result.push_back(nullptr);
#endif

					continue;
				}

				string pathToSource = makePathToSource(temp);

				if (filesystem::exists(pathToSource))
				{
					HMODULE handle = nullptr;

#ifdef __LINUX__
					handle = dlopen(pathToSource.data(), RTLD_LAZY);
#else
					handle = LoadLibraryA(pathToSource.data());
#endif
					result.push_back(handle);
				}
				else
				{
					throw file_manager::exceptions::FileDoesNotExistException(pathToSource);
				}

				if (!result.back())
				{
					throw exceptions::CantLoadSourceException(pathToSource);
				}
			}

			return result;
        }
    }
}
