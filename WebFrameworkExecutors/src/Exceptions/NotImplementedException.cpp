#include "NotImplementedException.h"

#include <format>

using namespace std;

namespace framework::exceptions
{
	NotImplementedException::NotImplementedException(string_view className, string_view methodName) :
		BadRequestException(format("Not implemented method {} in {}", methodName, className))
	{

	}
}
