#pragma once

#include "WebFrameworkConstants.h"

namespace framework
{
	namespace utility
	{
		/// @brief Make specific smart pointer for current build configuration
		/// @tparam T Any type
		/// @tparam ...Args Constructor arguments types
		/// @param ...args Constructor arguments
		/// @return smartPointer for current build configuration
		template<typename T, typename... Args>
		smartPointer<T> make_smartPointer(Args&&... args)
		{
			if constexpr (std::is_same_v<smartPointer<T>, std::unique_ptr<T>>)
			{
				return std::make_unique<T>(std::forward<Args>(args)...);
			}
			else if constexpr (std::is_same_v<smartPointer<T>, std::shared_ptr<T>>)
			{
				return std::make_shared<T>(std::forward<Args>(args)...);
			}
			else
			{
				throw std::runtime_error("Can't create smartPointer<T>");
			}
		}
	}
}
