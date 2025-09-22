#include "BaseConnectionData.h"

namespace framework::utility
{
	BaseConnectionData::BaseConnectionData(std::string_view ip, std::string_view port, DWORD timeout) :
		ip(ip),
		port(port),
		timeout(timeout)
	{

	}
}
