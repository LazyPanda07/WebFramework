#include "BadRequestException.h"

using namespace std;

namespace framework::exceptions
{
	BadRequestException::BadRequestException(string_view additionalInformation) :
		BaseExecutorException(format("Bad request from client. {}", additionalInformation))
	{

	}
}
