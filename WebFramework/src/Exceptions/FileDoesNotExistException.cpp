#include "FileDoesNotExistException.h"

#include "WebFrameworkConstants.h"

namespace framework
{
	namespace exceptions
	{
		FileDoesNotExistException::FileDoesNotExistException() :
			runtime_error(::exceptions::fileDoesNotExist)
		{

		}
	}
}