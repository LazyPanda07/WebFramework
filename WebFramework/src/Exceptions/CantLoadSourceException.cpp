#include "CantLoadSourceException.h"

#include "Framework/WebFrameworkConstants.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		CantLoadSourceException::CantLoadSourceException(string_view source) :
			BaseWebFrameworkException(format("{} {}. ", ::exceptions::cantLoadSource, source, ::exceptions::missingOtherDLLs))
		{

		}
	}
}
