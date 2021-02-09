#include "NotImplementedException.h"

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