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

			MultiByteToWideChar
			(
				sourceCodePage,
				NULL,
				source.data(),
				-1,
				tem.data(),
				size
			);

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

			WideCharToMultiByte
			(
				CP_UTF8,
				NULL,
				tem.data(),
				-1,
				result.data(),
				size,
				NULL,
				NULL
			);

			return result;
		}

		string cp1251ToUTF8(const string& source)
		{
			return toUTF8(source, 1251);
		}
	}
}
