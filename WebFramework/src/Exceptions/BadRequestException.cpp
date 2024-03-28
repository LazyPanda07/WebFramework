#include "BadRequestException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		BadRequestException::BadRequestException(string_view additionalInformation) :
			BaseExecutorException(format("{}. {}", ::exceptions::badRequest, additionalInformation.empty() ? "" : additionalInformation))
		{

		}
	}
}
