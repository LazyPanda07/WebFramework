#pragma once

#include "headers.h"

namespace framework
{
	namespace utility
	{
		/**
		* Convert symbols like space into % code
		*/
		WEB_FRAMEWORK_API_FUNCTION std::string encodeString(const std::string& source);

		/**
		* Convert % code into symbols
		*/
		WEB_FRAMEWORK_API_FUNCTION std::string decodeString(const std::string& source);
	}
}
