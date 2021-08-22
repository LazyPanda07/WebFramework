#pragma once

#include "headers.h"

#include "Exceptions/StringConversionException.h"

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
		WEB_FRAMEWORK_API_FUNCTION std::string toUTF8(const std::string& source, uint32_t sourceCodePage);

		/// <summary>
		/// Decode string from UTF8
		/// </summary>
		/// <param name="source">string to convert</param>
		/// <param name="resultCodePage">decoding code page</param>
		/// <returns>string in resultCodePage encoding</returns>
		/// <exception cref="framework::exceptions::StringConversion"></exception>
		WEB_FRAMEWORK_API_FUNCTION std::string fromUTF8(const std::string& source, uint32_t resultCodePage);

		/// <summary>
		/// Encode string from CP1251 to UTF8
		/// </summary>
		/// <param name="source">string to convert</param>
		/// <returns>string in UTF8 encoding</returns>
		/// <exception cref="framework::exceptions::StringConversion"></exception>
		WEB_FRAMEWORK_API_FUNCTION std::string cp1251ToUTF8(const std::string& source);

		/// @brief Encode string from UTF8 to CP1251
		/// @param source String to convert
		/// @return string in CP1251 encoding
		/// @exception framework::exceptions::StringConversion 
		WEB_FRAMEWORK_API_FUNCTION std::string utf8ToCP1251(const std::string& source);
	}
}
