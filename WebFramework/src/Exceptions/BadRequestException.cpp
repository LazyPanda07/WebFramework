#include "BadRequestException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		BadRequestException::BadRequestException(const string& additionalInformation) :
			BaseExecutorException(::exceptions::badRequest + (additionalInformation.empty() ? "" : ". " + additionalInformation))
		{

		}
	}
}
