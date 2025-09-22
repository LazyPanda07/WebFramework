#include "CantLoadSourceException.h"

#include "Framework/WebFrameworkConstants.h"

namespace framework::exceptions
{
	CantLoadSourceException::CantLoadSourceException(std::string_view source) :
		BaseWebFrameworkException(std::format("{} {}. ", ::exceptions::cantLoadSource, source, ::exceptions::missingOtherDLLs))
	{

	}
}
