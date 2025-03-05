#include "NotImplementedException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		NotImplementedException::NotImplementedException(string_view className, string_view methodName) :
			BadRequestException(format("{} method {} in {}", ::exceptions::notImplemented, methodName, className))
		{

		}
	}
}