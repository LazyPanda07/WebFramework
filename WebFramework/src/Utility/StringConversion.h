#pragma once

#if defined(CONVERSION_DLL) || defined(ALL_DLL)
#define WEB_FRAMEWORK_API extern "C" __declspec(dllexport)
#else
#define WEB_FRAMEWORK_API
#endif // CONVERSION_DLL

#include <string>

#include "WebFrameworkConstants.h"

namespace framework
{
	namespace utility
	{
		/// <summary>
		/// Encode string to UTF8
		/// </summary>
		/// <param name="source">string to convert</param>
		/// <param name="sourceCodePage">source encoding</param>
		/// <returns>string in UTF8 encoding</returns>
		/// <exception cref="framework::exceptions::StringConversion"></exception>
		WEB_FRAMEWORK_API std::string toUTF8(const std::string& source, unsigned int sourceCodePage);

		/// <summary>
		/// Decode string from UTF8
		/// </summary>
		/// <param name="source">string to convert</param>
		/// <param name="resultCodePage">decoding code page</param>
		/// <returns>string in resultCodePage encoding</returns>
		/// <exception cref="framework::exceptions::StringConversion"></exception>
		WEB_FRAMEWORK_API std::string fromUTF8(const std::string& source, unsigned int resultCodePage);

		/// <summary>
		/// Encode string from CP1251 to UTF8
		/// </summary>
		/// <param name="source">string to convert</param>
		/// <returns>string in UTF8 encoding</returns>
		/// <exception cref="framework::exceptions::StringConversion"></exception>
		WEB_FRAMEWORK_API std::string cp1251ToUTF8(const std::string& source);
	}
}
