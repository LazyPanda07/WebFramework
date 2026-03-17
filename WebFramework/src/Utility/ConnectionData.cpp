#include "Utility/ConnectionData.h"

namespace framework::utility
{
	ConnectionData::ConnectionData(std::string_view ip, std::string_view port, DWORD timeout) :
		ip(ip),
		port(port),
		timeout(timeout)
	{

	}
}
