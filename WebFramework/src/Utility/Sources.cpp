#include "Sources.h"

#include <Log.h>
#include <Exceptions/FileDoesNotExistException.h>

#include "DynamicLibraries.h"
#include "Exceptions/CantLoadSourceException.h"
#include "Framework/WebFrameworkConstants.h"
#include "Managers/RuntimesManager.h"

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

			if (type >= utility::LoadSourceType::dynamicLibrary && !std::filesystem::exists(pathToSource))
			{
				if (Log::isValid())
				{
					Log::error("Can't find source {}", "LogWebFrameworkSources", pathToSource);
				}

				throw file_manager::exceptions::FileDoesNotExistException(pathToSource);
			}

			if (type == LoadSourceType::dynamicLibrary)
			{
				source = utility::loadLibrary(pathToSource);

				if (!std::get<HMODULE>(source))
				{
					exceptionMessage = ::exceptions::missingOtherDLLs;
				}
			}
			else
			{
				if (std::optional<std::string> message = runtime::RuntimesManager::get().getRuntime(type).loadSource(pathToSource, source))
				{
					exceptionMessage = *message;
				}
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
