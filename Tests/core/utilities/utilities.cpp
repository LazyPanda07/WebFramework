#include "utilities.h"

#include "HTTPSNetwork.h"

#include "settings.h"

namespace utility
{
	streams::IOSocketStream createSocketStream()
	{
		static constexpr DWORD timeout = 0;

		return streams::IOSocketStream
		(
			useHTTPS ?
			std::make_unique<web::HTTPSNetwork>("127.0.0.1", "8080", timeout) :
			std::make_unique<web::HTTPNetwork>("127.0.0.1", "8080", timeout)
		);
	}
}
