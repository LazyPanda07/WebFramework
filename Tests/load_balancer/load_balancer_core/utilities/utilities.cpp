#include "utilities.h"

#include "HTTPSNetwork.h"

namespace utility
{
	streams::IOSocketStream createSocketStream(int64_t port, bool useHTTPS)
	{
		static constexpr DWORD timeout = 30'000;

		return streams::IOSocketStream
		(
			useHTTPS ?
			std::make_unique<web::HTTPSNetwork>("127.0.0.1", std::to_string(port), timeout) :
			std::make_unique<web::HTTPNetwork>("127.0.0.1", std::to_string(port), timeout)
		);
	}
}
