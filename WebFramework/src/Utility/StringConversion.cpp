#include "StringConversion.h"

#include <Windows.h>

using namespace std;

namespace framework
{
	namespace utility
	{
		wstring toUTF8(const string& source)
		{
			wstring result;
			int size = MultiByteToWideChar
			(
				CP_UTF8,
				NULL,
				source.data(),
				-1,
				nullptr,
				0
			);
			
			result.resize(size - 1);

			MultiByteToWideChar
			(
				CP_UTF8,
				NULL,
				source.data(),
				-1,
				result.data(),
				size
			);

			return result;
		}
	}
}
