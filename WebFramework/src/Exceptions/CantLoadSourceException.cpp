#include "CantLoadSourceException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		CantLoadSourceException::CantLoadSourceException(const string& source) :
			BaseWebFrameworkException(::exceptions::cantLoadSource + source + ". " + ::exceptions::missingOtherDLLs)
		{

		}
	}
}
