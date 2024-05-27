#pragma once

#include "IOSocketStream.h"

namespace utility
{
	streams::IOSocketStream createSocketStream(int64_t port, bool useHTTPS);
}
