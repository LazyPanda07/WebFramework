#include "BaseConnectionData.h"

using namespace std;

namespace framework
{
	namespace utility
	{
		BaseConnectionData::BaseConnectionData(const string& ip, const string& port) :
			ip(ip),
			port(port)
		{

		}

		BaseConnectionData::BaseConnectionData(string&& ip, string&& port) noexcept :
			ip(move(ip)),
			port(move(port))
		{

		}
	}
}
