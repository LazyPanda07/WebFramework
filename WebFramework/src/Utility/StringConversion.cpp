#include "StringConversion.h"

#include <Windows.h>

#include "Exceptions/StringConversionException.h"

using namespace std;

namespace framework
{
	namespace utility
	{
		string toUTF8(const string& source, unsigned int sourceCodePage)
		{
			string result;
			wstring tem;
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

		string fromUTF8(const string& source, unsigned int resultCodePage)
		{
			string result;
			wstring tem;
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

		string cp1251ToUTF8(const string& source)
		{
			return toUTF8(source, 1251);
		}
	}
}
