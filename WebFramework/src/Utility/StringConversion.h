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
		inline std::string toUTF8(const std::string& source, uint32_t sourceCodePage)
		{
			std::string result;
			std::wstring tem;
			int size = MultiByteToWideChar
			(
				sourceCodePage,
				NULL,
				source.data(),
				-1,
				nullptr,
				NULL
			);

			if (!size)
			{
				throw exceptions::StringConversionException();
			}

			tem.resize(static_cast<size_t>(size) - 1);

			if (!MultiByteToWideChar
			(
				sourceCodePage,
				NULL,
				source.data(),
				-1,
				tem.data(),
				size
			))
			{
				throw exceptions::StringConversionException();
			}

			size = WideCharToMultiByte
			(
				CP_UTF8,
				NULL,
				tem.data(),
				-1,
				nullptr,
				NULL,
				NULL,
				NULL
			);

			if (!size)
			{
				throw exceptions::StringConversionException();
			}

			result.resize(static_cast<size_t>(size) - 1);

			if (!WideCharToMultiByte
			(
				CP_UTF8,
				NULL,
				tem.data(),
				-1,
				result.data(),
				size,
				NULL,
				NULL
			))
			{
				throw exceptions::StringConversionException();
			}

			return result;
		}

		/// <summary>
		/// Decode string from UTF8
		/// </summary>
		/// <param name="source">string to convert</param>
		/// <param name="resultCodePage">decoding code page</param>
		/// <returns>string in resultCodePage encoding</returns>
		/// <exception cref="framework::exceptions::StringConversion"></exception>
		inline std::string fromUTF8(const std::string& source, uint32_t resultCodePage)
		{
			std::string result;
			std::wstring tem;
			int size = MultiByteToWideChar
			(
				CP_UTF8,
				NULL,
				source.data(),
				-1,
				nullptr,
				NULL
			);

			if (!size)
			{
				throw exceptions::StringConversionException();
			}

			tem.resize(static_cast<size_t>(size) - 1);

			if (!MultiByteToWideChar
			(
				CP_UTF8,
				NULL,
				source.data(),
				-1,
				tem.data(),
				size
			))
			{
				throw exceptions::StringConversionException();
			}

			size = WideCharToMultiByte
			(
				resultCodePage,
				NULL,
				tem.data(),
				-1,
				nullptr,
				NULL,
				NULL,
				NULL
			);

			if (!size)
			{
				throw exceptions::StringConversionException();
			}

			result.resize(static_cast<size_t>(size) - 1);

			if (!WideCharToMultiByte
			(
				resultCodePage,
				NULL,
				tem.data(),
				-1,
				result.data(),
				size,
				NULL,
				NULL
			))
			{
				throw exceptions::StringConversionException();
			}

			return result;
		}

		/// <summary>
		/// Encode string from CP1251 to UTF8
		/// </summary>
		/// <param name="source">string to convert</param>
		/// <returns>string in UTF8 encoding</returns>
		/// <exception cref="framework::exceptions::StringConversion"></exception>
		inline std::string cp1251ToUTF8(const std::string& source)
		{
			return toUTF8(source, 1251);
		}
	}
}
