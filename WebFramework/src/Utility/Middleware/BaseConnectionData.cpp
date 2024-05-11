#include "BaseConnectionData.h"

using namespace std;

namespace framework
{
	namespace utility
	{
		BaseConnectionData::BaseConnectionData(const string& ip, const string& port, DWORD timeout) :
			ip(ip),
			port(port),
			timeout(timeout)
		{

		}

		BaseConnectionData::BaseConnectionData(string&& ip, string&& port, DWORD timeout) noexcept :
			ip(move(ip)),
			port(move(port)),
			timeout(timeout)
		{

		}
	}
}
