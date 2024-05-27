#include "BaseConnectionData.h"

using namespace std;

namespace framework
{
	namespace utility
	{
		BaseConnectionData::BaseConnectionData(string_view ip, string_view port, DWORD timeout) :
			ip(ip),
			port(port),
			timeout(timeout)
		{

		}
	}
}
