#include "baseConnectionData.h"

using namespace std;

namespace framework
{
	namespace utility
	{
		baseConnectionData::baseConnectionData(const string& ip, const string& port) :
			ip(ip),
			port(port)
		{

		}

		baseConnectionData::baseConnectionData(string&& ip, string&& port) noexcept :
			ip(move(ip)),
			port(move(port))
		{

		}
	}
}
