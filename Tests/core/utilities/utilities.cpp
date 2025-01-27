#include "utilities.h"

#include "HTTPSNetwork.h"

#include "settings.h"

namespace utility
{
	streams::IOSocketStream createSocketStream()
	{
		static constexpr DWORD timeout = 600'000;

		return useHTTPS ?
			streams::IOSocketStream::createStream<web::HTTPSNetwork>("127.0.0.1", "8080", timeout) :
			streams::IOSocketStream::createStream<web::HTTPNetwork>("127.0.0.1", "8080", timeout);
	}
}
