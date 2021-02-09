#include "CantLoadSourceException.h"

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
