#include "CantLoadSourceException.h"

#include "WebFrameworkConstants.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		CantLoadSourceException::CantLoadSourceException(const string& source) :
			runtime_error(::exceptions::cantLoadSource + source + ". " + ::exceptions::missingOtherDLLs)
		{

		}
	}
}
