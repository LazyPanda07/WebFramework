#include "Utility/Sources.h"

#include <Log.h>
#include <Exceptions/FileDoesNotExistException.h>

#include "Utility/DynamicLibraries.h"
#include "Exceptions/CantLoadSourceException.h"
#include "Framework/WebFrameworkConstants.h"
#include "Managers/RuntimesManager.h"
#include "Runtimes/CXXRuntime.h"
#include "Runtimes/CCRuntime.h"
#include "Utility/Utils.h"

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
				utility::logAndThrowException<logging::message::cantFindSource, logging::category::sources>(pathToSource);
			}

			if (type == LoadSourceType::dynamicLibrary)
			{
				std::optional<std::string> cxxErrorMessage = runtime::RuntimesManager::get().getRuntime<runtime::CXXRuntime>().loadSource(pathToSource, source);

				if (cxxErrorMessage)
				{
					std::optional<std::string> ccErrorMessage = runtime::RuntimesManager::get().getRuntime<runtime::CCRuntime>().loadSource(pathToSource, source);

					if (ccErrorMessage)
					{
						exceptionMessage = ::exceptions::missingOtherDLLs;
					}
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
				utility::logAndThrowException<logging::message::cantLoadSource, logging::category::sources>(pathToSource, exceptionMessage);
			}

			result.emplace_back(std::move(source), pathToSource);
		}

		return result;
	}
}
