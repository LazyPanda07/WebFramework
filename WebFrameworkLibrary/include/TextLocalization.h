#pragma once

/// @file TextLocalization.h
/// @brief BaseTextLocalization with std::string

#include "BaseTextLocalization.h"

namespace localization
{
	/// @brief TextLocalization with string
	using TextLocalization = localization::BaseTextLocalization<char>;
}
