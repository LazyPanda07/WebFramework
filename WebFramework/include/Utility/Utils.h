#pragma once

#include <Log.h>

#include "Framework/WebFrameworkConstants.h"

namespace framework::utility
{
	template<const auto& Format, const auto& Category, typename... Args>
	void logAndThrowException(Args&&... args) requires log_utils::LogFormat<Format, Args...>;
}

namespace framework::utility
{
	template<const auto& Format, const auto& Category, typename... Args>
	void logAndThrowException(Args&&... args) requires log_utils::LogFormat<Format, Args...>
	{
		if (Log::isValid())
		{
			Log::error<Format, Category>(std::forward<Args>(args)...);
		}

		throw std::runtime_error(std::format(Format, std::forward<Args>(args)...));
	}
}
