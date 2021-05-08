#include "StringConversionException.h"

#include <string>

#include <Windows.h>

using namespace std;

namespace framework
{
	namespace exceptions
	{
		StringConversionException::StringConversionException() :
			BaseWebFrameworkException(::exceptions::stringConversionErrorCode + to_string(GetLastError()))
		{

		}
	}
}
