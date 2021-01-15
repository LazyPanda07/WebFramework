#include "MiddlewareServer.h"

using namespace std;

namespace framework
{
	namespace middleware
	{
		MiddlewareServer::MiddlewareServer(const string& port, const string& ip, DWORD timeout) :
			BaseTCPServer(port, ip, timeout, false)
		{

		}
	}
}
