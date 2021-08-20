#pragma once

#include "WebFrameworkConstants.h"

#include "JSONUtility.h"

#ifdef DLL_BUILD
#define WEB_FRAMEWORK_API_FUNCTION __declspec(dllexport) 
#else
#define WEB_FRAMEWORK_API_FUNCTION 
#endif // DLL_BUILD

namespace framework
{
	namespace utility
	{
		/// @brief Copy top level JSON object. Supported simple array(null, int, string, double)
		/// @param json JSON to copy
		/// @return Copied JSON
		json::utility::objectSmartPointer<json::utility::jsonObject> copyJSON(const json::utility::objectSmartPointer<json::utility::jsonObject>& json);
	}
}
