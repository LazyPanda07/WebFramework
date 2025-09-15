#include "DynamicPagesSyntaxException.h"

#include "Framework/WebFrameworkConstants.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		DynamicPagesSyntaxException::DynamicPagesSyntaxException(string_view syntaxError) :
			BaseWebFrameworkException(syntaxError)
		{

		}
	}
}
