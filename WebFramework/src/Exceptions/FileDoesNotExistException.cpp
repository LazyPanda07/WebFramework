#include "FileDoesNotExistException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		FileDoesNotExistException::FileDoesNotExistException(const string& filename) :
			runtime_error(filename + " " + ::exceptions::fileDoesNotExist)
		{

		}
	}
}