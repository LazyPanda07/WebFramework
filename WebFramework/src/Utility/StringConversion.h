#pragma once

#include <string>

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
		std::string toUTF8(const std::string& source, unsigned int sourceCodePage);

		/// <summary>
		/// Encode string from CP1251 to UTF8
		/// </summary>
		/// <param name="source">string to convert</param>
		/// <returns>string in UTF8 encoding</returns>
		/// <exception cref="framework::exceptions::StringConversion"></exception>
		std::string cp1251ToUTF8(const std::string& source);
	}
}
