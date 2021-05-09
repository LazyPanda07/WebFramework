#include "DynamicPagesSyntaxException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		DynamicPagesSyntaxException::DynamicPagesSyntaxException(const string& syntaxError) :
			BaseWebFrameworkException(syntaxError)
		{

		}
	}
}
