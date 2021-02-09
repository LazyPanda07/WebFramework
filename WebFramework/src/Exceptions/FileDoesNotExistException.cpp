#include "FileDoesNotExistException.h"

using namespace std;

namespace framework
{
	namespace exceptions
	{
		FileDoesNotExistException::FileDoesNotExistException(const string& fileName) :
			runtime_error(fileName + " " + ::exceptions::fileDoesNotExist)
		{

		}
	}
}