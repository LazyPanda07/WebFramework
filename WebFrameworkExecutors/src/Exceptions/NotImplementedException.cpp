#include "NotImplementedException.h"

#include <format>

namespace framework::exceptions
{
	NotImplementedException::NotImplementedException(std::string_view className, std::string_view methodName) :
		BadRequestException(std::format("Not implemented method {} in {}", methodName, className))
	{

	}
}
