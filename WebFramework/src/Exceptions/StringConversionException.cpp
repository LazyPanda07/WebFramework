#include "StringConversionException.h"

#include <string>

#include <Windows.h>

#include "WebFrameworkConstants.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		StringConversionException::StringConversionException() :
			runtime_error(::exceptions::stringConversionErrorCode + to_string(GetLastError()))
		{

		}
	}
}
