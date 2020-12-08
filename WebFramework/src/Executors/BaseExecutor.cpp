#include "BaseExecutor.h"

using namespace std;

namespace framework
{
	void BaseExecutor::doPost(HTTPRequest&& request, string& response)
	{
		throw exceptions::NotImplementedException();
	}

	void BaseExecutor::doGet(HTTPRequest&& request, string& response)
	{
		throw exceptions::NotImplementedException();
	}
}
