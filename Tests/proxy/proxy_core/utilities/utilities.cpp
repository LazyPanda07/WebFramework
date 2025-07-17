#include "utilities.h"

#include "HTTPSNetwork.h"

namespace utility
{
	streams::IOSocketStream createSocketStream(int64_t port, bool useHTTPS)
	{
		return useHTTPS ?
			streams::IOSocketStream::createStream<web::HTTPSNetwork>("127.0.0.1", std::to_string(port)) :
			streams::IOSocketStream::createStream<web::HTTPNetwork>("127.0.0.1", std::to_string(port));
	}
}
