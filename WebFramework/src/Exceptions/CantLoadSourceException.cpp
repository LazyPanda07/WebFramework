#include "CantLoadSourceException.h"

#include "Framework/WebFrameworkConstants.h"

namespace framework::exceptions
{
	CantLoadSourceException::CantLoadSourceException(std::string_view source, std::string_view additionalInformation) :
		BaseWebFrameworkException(std::format("{} {}. ", ::exceptions::cantLoadSource, source, additionalInformation))
	{

	}
}
