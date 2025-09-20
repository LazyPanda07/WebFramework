#include "DynamicPagesSyntaxException.h"

#include "Framework/WebFrameworkConstants.h"

namespace framework::exceptions
{
	DynamicPagesSyntaxException::DynamicPagesSyntaxException(std::string_view syntaxError) :
		BaseWebFrameworkException(syntaxError)
	{

	}
}
