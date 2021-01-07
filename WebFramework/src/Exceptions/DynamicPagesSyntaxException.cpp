#include "DynamicPagesSyntaxException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		DynamicPagesSyntaxException::DynamicPagesSyntaxException(const string& syntaxError) :
			runtime_error(syntaxError)
		{

		}
	}
}
