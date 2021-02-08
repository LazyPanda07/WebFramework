#include "NotImplementedException.h"

#include "WebFrameworkConstants.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		NotImplementedException::NotImplementedException() :
			BadRequestException(::exceptions::notImplemented)
		{

		}
	}
}