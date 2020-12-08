#include "BaseExecutor.h"

using namespace std;

namespace framework
{
	void BaseExecutor::doPost(HTTPRequest&& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException();
	}

	void BaseExecutor::doGet(HTTPRequest&& request, HTTPResponse& response)
	{
		throw exceptions::NotImplementedException();
	}
}
