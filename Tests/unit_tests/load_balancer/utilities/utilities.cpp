#include "utilities.h"

#include <chrono>

#include <Http/HttpsNetwork.h>

using namespace std::chrono_literals;

namespace utility
{
	streams::IOSocketStream createSocketStream(int64_t port, bool useHTTPS)
	{
		return useHTTPS ?
			streams::IOSocketStream::createStream<web::http::HttpsNetwork>("127.0.0.1", std::to_string(port), 1h) :
			streams::IOSocketStream::createStream<web::http::HttpNetwork>("127.0.0.1", std::to_string(port), 1h);
	}
}
