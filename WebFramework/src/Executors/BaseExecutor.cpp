#include "BaseExecutor.h"

using namespace std;

namespace framework
{
	void BaseExecutor::doPost(web::HTTPParser&& request, string& response)
	{
		throw exceptions::NotImplementedException();
	}

	void BaseExecutor::doGet(web::HTTPParser&& request, string& response)
	{
		throw exceptions::NotImplementedException();
	}
}
